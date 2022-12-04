#include "font.h"
#include "Core/logger.h"
#include "Core/application.h"
#include "Platform/OpenGL/gl_graphics_api.h"


namespace Lynx::Graphics {


	FontManager::FontManager()
	{
		if(FT_Init_FreeType(&ft))
			log_error("Freetype: Failed to initialize");
			
		defaultShader = Application::GetSingleton()->GetResourceManager()->LoadShader(
				"res/shaders/text.vert", "res/shaders/text.frag"); // These shaders should be hardcoded
		defaultFont = AddFont("res/fonts/unifont.ttf", 16);
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

		return AddFont(name, size);
	}

	Font* FontManager::AddFont(std::string name, int size)
	{
		Font* f = new Font(&ft, defaultShader, name, size);
		loadedFonts.push_back(f);
		return f;
	}

	Font::Font(FT_Library *ft, std::shared_ptr<Shader> shader, std::string name, int size) :
		name(name), size(size), shader(shader)
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
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
		VAO->Unbind();
		VBO->Unbind();
	}

	void Font::render(std::string text, float x, float y, float scale, glm::vec3 color, glm::mat4 projection)
	{
		shader->Use();
		shader->SetUniform("color", color);
		shader->SetUniform("projection", projection);
		glActiveTexture(GL_TEXTURE0);
		VAO->Bind();

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

				{ xpos,		ypos + h,	0.0,	0.0	},
				{ xpos + w,	ypos,		1.0,	1.0	},
				{ xpos + w, ypos + h,	1.0,	0.0	}
			};

			glBindTexture(GL_TEXTURE_2D, g.textureID);

			VBO->Bind();
			VBO->AddData(vertices, sizeof(vertices));
			VBO->Unbind();

			glDrawArrays(GL_TRIANGLES, 0, 6);
			x += (g.advance >> 6) * scale;
		}
		VAO->Unbind();
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
