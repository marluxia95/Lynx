#ifndef LYNX_FONT_H
#define LYNX_FONT_H

#include <stdio.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <glm/glm.hpp>
#include <map>

#include "shader.h"
#include "vertex_array.h"
#include "buffers.h"

namespace Lynx::Graphics {

	struct Glyph
	{
		unsigned int textureID;
		glm::ivec2 size;
		glm::ivec2 bearing;
		long int advance;
	};

	struct Font
	{
		Font(FT_Library *ft, std::shared_ptr<Shader> shader, std::string name, int size);

		FT_Face face;
		std::map<char, Glyph> glyphs;
		std::string name;
		int size;
		std::shared_ptr<Shader> shader;
		void render(std::string text, float x, float y, float scale, glm::vec3 color, glm::mat4 projection);
		std::unique_ptr<VertexArray>  VAO;
		std::unique_ptr<VertexBuffer> VBO;
	};

	// TODO : Use the resource manager to load fonts instead
	class FontManager {
	public:
		FontManager();
		~FontManager();

		Font* GetFont(std::string name, int size);
		Font* AddFont(std::string name, int size);
		Font* GetDefaultFont() { return defaultFont; }
	private:
		std::vector<Font*> loadedFonts;
		std::shared_ptr<Shader> defaultShader;
		Font* defaultFont;
		FT_Library ft;
	};

}

#endif
