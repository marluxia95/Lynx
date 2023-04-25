#include "draw.h"
#include "Core/logger.h"
#include "Core/application.h"
#include "Platform/OpenGL/gl_graphics_api.h"


namespace Lynx::Graphics {
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
		if(FT_New_Face(*ft, name.c_str(), 0, &face))
			log_error("Freetype : Unable to load font %s", name.c_str());

		FT_Set_Pixel_Sizes(face, 0, size);
		
		glPixelStorei(GL_UNPACK_ALIGNMENT,1);
		for(uint8_t c = 0; c < 128; c++)
		{
			if(FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				log_error("Freetype : Failed to load glyph from font %s", name.c_str());
				continue;
			}
			GLuint texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
					GL_TEXTURE_2D,
					0,
					GL_RED,
					face->glyph->bitmap.width,
					face->glyph->bitmap.rows,
					0,
					GL_RED,
					GL_UNSIGNED_BYTE,
					face->glyph->bitmap.buffer
					);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			Glyph glyph = {
				texture,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left,  face->glyph->bitmap_top ),
				face->glyph->advance.x
			};
			glyphs.insert(std::pair<char, Glyph>(c, glyph));
		}
		log_debug("Loaded typeface %s (%dpx)", name.c_str(), size);
		FT_Done_Face(face);

		VAO = VertexArray::Create();
		VBO = VertexBuffer::Create();
		VBO->DynamicDraw(true);
		VBO->SetData(NULL, sizeof(float) * 6 *4);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(0);
		VAO->Unbind();
		VBO->Unbind();
	}
	
	FontManager::FontManager()
	{
		if(FT_Init_FreeType(&ft))
			log_error("Freetype: Failed to initialize");
			
		defaultFont = LoadFont("res/fonts/ProggyTiny.ttf", 32);
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
		//	"	if(texture(text, TexCoords).r < 0.5) discard;\n"
			"	vec4 sampled = vec4(1.0f, 1.0f, 1.0f, texture(text, TexCoords).r);\n"
			"	color = vec4(textColor, alpha) * sampled;\n"
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

	void DrawImg(float x, float y, std::shared_ptr<TextureBase> img, float width, float height, glm::vec4 color) 
	{
		glm::mat4 projection = getProjection();
		
		genericNoTextureShader->Use();
		genericNoTextureShader->SetUniform("projection", projection);
		RendererAPI::UseTexture(0);
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
		img->Use();
		RendererAPI::DrawArrays(6);

		ui_draw_VAO->Unbind();
		RendererAPI::BindTexture(TEXTURE_2D, 0);
	
	}

	void DrawFill(float x, float y, float width, float height, glm::vec4 color) 
	{
		glm::mat4 projection = getProjection();
		
		genericShader->Use();
		genericShader->SetUniform("projection", projection);

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

	void DrawString(float x, float y, std::string text, Font* font, float scale, glm::vec3 color, float alpha)
	{
		glm::mat4 projection = getProjection();
		std::map<char, Glyph> glyphs = font->glyphs;
		genericFontShader->Use();
		genericFontShader->SetUniform("textColor", color);
		genericFontShader->SetUniform("alpha", alpha);
		genericFontShader->SetUniform("projection", projection);
		RendererAPI::UseTexture(0);
		font->VAO->Bind();

		std::string::const_iterator c;
		for ( c = text.begin(); c != text.end(); c++ )
		{
			Glyph g = glyphs[*c];

			GLfloat xpos = x + g.bearing.x * scale;
			GLfloat ypos = y - (g.size.y - g.bearing.y) * scale;

			GLfloat w = g.size.x * scale;
			GLfloat h = g.size.y * scale;

			GLfloat vertices[6][4] = {
				{ xpos,		ypos + h,	0.0,	0.0 },
				{ xpos, 	ypos,		0.0,	1.0 },
				{ xpos + w,	ypos,		1.0,	1.0 },

				{ xpos,		ypos + h,	0.0,	0.0 },
				{ xpos + w,	ypos,		1.0,	1.0 },
				{ xpos + w, 	ypos + h,	1.0,	0.0 }
			};

			RendererAPI::BindTexture(TEXTURE_2D, g.textureID);
			font->VBO->Bind();
			font->VBO->AddData(vertices, sizeof(vertices));
			font->VBO->Unbind();

			RendererAPI::DrawArrays(6);
			x += (g.advance >> 6) * scale;
		}
		font->VAO->Unbind();
		RendererAPI::BindTexture(TEXTURE_2D, 0);
		API_CheckErrors();
	}

	void DrawFree()
	{
		delete genericFontShader;
		delete genericShader;
		delete genericNoTextureShader;
	}
}
