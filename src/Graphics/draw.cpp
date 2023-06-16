#include "draw.h"
#include "Core/logger.h"
#include "Core/application.h"
#include "gl_graphics_api.h"
#define MAXWIDTH 1024

namespace Lynx {
	Shader* genericShader = NULL;
	Shader* genericNoTextureShader = NULL;
	Shader* genericFontShader = NULL;
	std::unique_ptr<VertexArray> ui_draw_VAO = NULL;
	std::unique_ptr<VertexBuffer> ui_draw_VBO = NULL;

	struct prev_drawstate {
		bool enable_cull_face;
		bool enable_depth_test;
		bool enable_stencil_test;
		bool enable_scissor_test;
	};

	prev_drawstate last_state;

	Font::Font(FT_Library *ft, const std::string name, int size) :
		name(name), size(size)
	{
		log_debug("Font::Font : Generating atlas");
		if(FT_New_Face(*ft, name.c_str(), 0, &face))
			log_error("Freetype : Unable to load font %s", name.c_str());

		FT_Set_Pixel_Sizes(face, 0, size);

		FT_GlyphSlot g = face->glyph;
		unsigned int roww = 0;
		unsigned int rowh = 0;

		memset(c, 0, sizeof c);

		for(int i = 32; i < 128; i++) {
			if(FT_Load_Char(face, i, FT_LOAD_RENDER)) {
				log_error("Freetype : Unable to load character %c", i);
				continue;
			}
			if(roww + g->bitmap.width + 1 >= MAXWIDTH) {
				w = std::max(w, roww);
				h += rowh;
				roww = 0;
				rowh = 0;
			}

			roww += g->bitmap.width + 1;
			rowh = std::max(rowh, g->bitmap.rows);
		}

		w = std::max(w, roww);
		h += rowh;

		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


		int ox = 0;
		int oy = 0;

		rowh = 0;

		for(int i = 32; i < 128; i++) {
			if(FT_Load_Char(face, i, FT_LOAD_RENDER))
				continue;
			if(ox + g->bitmap.width + 1 >= MAXWIDTH) {
				oy += rowh;
				rowh = 0;
				ox = 0;
			}

            glCheckError();
			glTexSubImage2D(GL_TEXTURE_2D, 0, ox, oy, g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);

			// Glyph advance
			c[i].ax = g->advance.x >> 6;
			c[i].ay = g->advance.y >> 6;

			// Bitmap metrics
                        c[i].bw = g->bitmap.width;
			c[i].bh = g->bitmap.rows;

			// Glyph bearing
			c[i].bl = g->bitmap_left;
			c[i].bt = g->bitmap_top;

			// Texture offset
			c[i].tx = ox / (float)w;
			c[i].ty = oy / (float)h;

			rowh = std::max(rowh, g->bitmap.rows);
			ox += g->bitmap.width + 1;
		}
		FT_Done_Face(face);

		VAO = VertexArray::Create();
                VBO = VertexBuffer::Create();
		VBO->DynamicDraw(true);
		VBO->SetData(NULL, sizeof(float)* 6*4);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
		glEnableVertexAttribArray(0);
		VAO->Unbind();
		VBO->Unbind();

		glCheckError();

		log_debug("generated font atlas %s", name.c_str());
	}


	FontManager::FontManager()
	{
		if(FT_Init_FreeType(&ft))
			log_error("Freetype: Failed to initialize");

		defaultFont = LoadFont("res/fonts/ProggyTiny.ttf", 64);
	}

	FontManager::~FontManager()
	{
		for(auto& i : loadedFonts)
		{
			delete i;
		}
		FT_Done_FreeType(ft);
	}

	Font* FontManager::GetFont(std::string name, int size)
	{
		for (auto &f : loadedFonts )
			if(f->name == name && f->size == size)
				return f;

		return LoadFont(name, size);
	}

	Font* FontManager::LoadFont(std::string name, int size)
	{
		Font* f = new Font(&ft, name, size);
		loadedFonts.push_back(f);
		return f;
	}


	void DrawInit()
	{
		//auto resManager = Application::GetSingleton()->GetResourceManager();

		const char* genericFontShader_vertex_source = 
			"#version 330 core\n"
			"layout (location = 0) in vec4 vertex;\n"
			"out vec2 TexCoords;\n"
			"uniform mat4 projection;\n"
			"void main()\n"
			"{\n"
			"	gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);\n"
			"	TexCoords = vertex.zw;\n"
			"}\n";

		const char* genericFontShader_fragment_source = 
			"#version 330 core\n"
			"in vec2 TexCoords;\n"
			"out vec4 color;\n"
			"uniform sampler2D text;\n"
			"uniform vec3 textColor;\n"
			"uniform float alpha;\n"
			"void main()\n"
			"{\n"
			"	color = vec4(textColor, alpha * texture(text, TexCoords));\n"
			"}\n";

		const char* genericShader_vertex_source =
			"#version 330 core\n"
			"layout (location = 0) in vec2 position;\n"
			"layout (location = 1) in vec4 color;\n"
			"layout (location = 2) in vec2 uv;\n"
			"out vec2 frag_uv;\n"
			"out vec4 frag_color;\n"
			"uniform mat4 projection;\n"
			"void main()\n"
			"{\n"
			"	gl_Position = projection * vec4(position.xy, 0, 1);\n"
			"	frag_uv = uv;\n"
			"	frag_color = color;\n"
			"}\n";

		const char* genericShader_fragment_source =
			"#version 330 core\n"
			"in vec2 frag_uv;\n"
			"in vec4 frag_color;\n"
			"out vec4 color;\n"
			"uniform sampler2D img;\n"
			"void main()\n"
			"{\n"
			"	color = texture(img, frag_uv) * frag_color;\n"
			"}\n";

		const char* genericNoTextureShader_vertex_source =
			"#version 330 core\n"
			"layout (location = 0) in vec2 position;\n"
			"layout (location = 1) in vec4 color;\n"
			"out vec4 frag_color;\n"
			"uniform mat4 projection;\n"
			"void main()\n"
			"{\n"
			"	gl_Position = projection * vec4(position.xy, 0, 1);\n"
			"	frag_color = color;\n"
			"}\n";

		const char* genericNoTextureShader_fragment_source = 
			"#version 330 core\n"
			"in vec4 frag_color;\n"
			"out vec4 color;\n"
			"void main()\n"
			"{\n"
			"	color = frag_color;\n"
			"}\n";


		genericShader = new Shader();
		genericShader->PushRawSource("generic", genericShader_vertex_source, SHADER_VERTEX);
		genericShader->PushRawSource("generic", genericShader_fragment_source, SHADER_FRAGMENT);
		genericShader->Link();

		genericFontShader = new Shader();
		genericFontShader->PushRawSource("genericfont", genericFontShader_vertex_source, SHADER_VERTEX);
		genericFontShader->PushRawSource("genericfont", genericFontShader_fragment_source, SHADER_FRAGMENT);
		genericFontShader->Link();

		genericNoTextureShader = new Shader();
		genericNoTextureShader->PushRawSource("genericnotexture", genericNoTextureShader_vertex_source, SHADER_VERTEX);
		genericNoTextureShader->PushRawSource("genericnotexture", genericNoTextureShader_fragment_source, SHADER_FRAGMENT);
		genericNoTextureShader->Link();

		ui_draw_VAO = VertexArray::Create();
		ui_draw_VBO = VertexBuffer::Create();

		ui_draw_VBO->DynamicDraw(true);
		ui_draw_VBO->SetData(NULL, sizeof(float) * 6 * 8);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(2*sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6*sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
		ui_draw_VAO->Unbind();
		ui_draw_VBO->Unbind();
	}

	glm::mat4 getProjection()
	{
		auto appInstance = Application::GetSingleton();
		float scrWidth = (float)appInstance->GetResolutionWidth();
		float scrHeight = (float)appInstance->GetResolutionHeight();
		return glm::ortho(0.0f, scrWidth, 0.0f, scrHeight);
	}

	void DrawInitRender()
	{
		last_state.enable_cull_face = glIsEnabled(GL_CULL_FACE);
		last_state.enable_depth_test = glIsEnabled(GL_DEPTH_TEST);
		last_state.enable_stencil_test = glIsEnabled(GL_STENCIL_TEST);
		last_state.enable_scissor_test = glIsEnabled(GL_SCISSOR_TEST);
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_STENCIL_TEST);
		glEnable(GL_SCISSOR_TEST);
	}

	void DrawFinishRender()
	{
		if(last_state.enable_cull_face) glEnable(GL_CULL_FACE); else glDisable(GL_CULL_FACE);
		if(last_state.enable_depth_test) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST);
		if(last_state.enable_stencil_test) glEnable(GL_STENCIL_TEST); else glDisable(GL_STENCIL_TEST);
		if(last_state.enable_scissor_test) glEnable(GL_SCISSOR_TEST); else glDisable(GL_SCISSOR_TEST);
	}

	void DrawTex(float x, float y, unsigned int texture_id, float width, float height, glm::vec4 color) 
	{
		glm::mat4 projection = getProjection();
		genericNoTextureShader->Use();
		genericNoTextureShader->SetUniform("projection", projection);
		RendererAPI::UseTexture(0);
		RendererAPI::BindTexture(TEXTURE_2D, texture_id);
		ui_draw_VAO->Bind();

		GLfloat vertices[] = {
			x,		y + height,	color.x,	color.y,	color.z,	color.w,	0.0f,	0.0f,
			x,		y,		color.x,	color.y,	color.z,	color.w,	0.0f,	1.0f,
			x + width,	y,		color.x,	color.y,	color.z,	color.w,	1.0f,	1.0f,

			x,		y + height,	color.x,	color.y,	color.z,	color.w,	0.0f,	0.0f,
			x + width,	y,		color.x,	color.y,	color.z,	color.w,	1.0f,	1.0f,
			x + width,	y + height,	color.x,	color.y,	color.z,	color.w,	1.0f,	0.0f
		};

		ui_draw_VBO->Bind();
		ui_draw_VBO->AddData(vertices, sizeof(vertices));
		ui_draw_VBO->Unbind();
		RendererAPI::DrawArrays(6);

		ui_draw_VAO->Unbind();
	}

	void DrawFill(float x, float y, float width, float height, glm::vec4 color) 
	{
		glm::mat4 projection = getProjection();

		genericShader->Use();
		genericShader->SetUniform("projection", projection);

		ui_draw_VAO->Bind();
		GLfloat vertices[] = {
			x,y + height,	color.x,	color.y,	color.z,	color.w,	0.0f,	0.0f,
			x,		y,color.x,	color.y,	color.z,	color.w,	0.0f,	1.0f,
			x + width,	y,		color.x,	color.y,	color.z,	color.w,	1.0f,	1.0f,

			x,		y + height,	color.x,	color.y,	color.z,	color.w,	0.0f,	0.0f,
			x + width,	y,		color.x,	color.y,	color.z,	color.w,	1.0f,	1.0f,
			x + width,	y + height,	color.x,	color.y,	color.z,	color.w,	1.0f,	0.0f
		};

		ui_draw_VBO->Bind();
		ui_draw_VBO->AddData(vertices, sizeof(vertices));
		ui_draw_VBO->Unbind();
		RendererAPI::DrawArrays(6);

		ui_draw_VAO->Unbind();
	}

	void DrawString(float x, float y, const char *text, Font* font, float scale, glm::vec3 color, float alpha)
	{
		struct point {
			float x;
			float y;
			float tx;
			float ty;
		};

		glm::mat4 projection = getProjection();
		Glyph *glyphs = font->c;

		genericFontShader->Use();
		genericFontShader->SetUniform("textColor", color);
		genericFontShader->SetUniform("alpha", alpha);
		genericFontShader->SetUniform("projection", projection);
		font->VAO->Bind();

		float atlas_w = font->w;
		float atlas_h = font->h;

		Glyph g;
		float xpos, ypos, w, h, tbw, tbh;

		int n = 0;
		size_t n_verts = 6 * strlen(text);
		point vertices[n_verts];

		for (const char *c = text; *c; c++) {
			g = glyphs[*c];

			xpos = x + g.bl * scale;
			ypos = y - (g.bh - g.bt) * scale;

			w = g.bw * scale;
			h = g.bh * scale;
			tbw = g.bw / atlas_w;// Texture coordinates for bearing
			tbh = g.bh / atlas_h; 

			x += g.ax * scale;
			y += g.ay * scale;

			vertices[n++] = {xpos, ypos + h, g.tx, g.ty      };
			vertices[n++] = {xpos, ypos,     g.tx, g.ty + tbh};
			vertices[n++] = {xpos+w, ypos, g.tx+tbw, g.ty + tbh};

			vertices[n++] = {xpos, ypos + h, g.tx, g.ty};
			vertices[n++] = {xpos + w, ypos, g.tx+tbw, g.ty+tbh};
			vertices[n++] = {xpos + w, ypos + h, g.tx+tbw, g.ty};
		}

		RendererAPI::UseTexture(0);
		RendererAPI::BindTexture(TEXTURE_2D, font->tex);

		font->VBO->SetData(vertices, sizeof(vertices));
		API_CheckErrors();
		RendererAPI::DrawArrays(n);
		font->VBO->Unbind();
		font->VAO->Unbind();
		API_CheckErrors();
	}

	void DrawFree()
	{
		delete genericFontShader;
		delete genericShader;
		delete genericNoTextureShader;
	}
}
