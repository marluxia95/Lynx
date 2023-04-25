#ifndef LYNX_DRAW_H
#define LYNX_DRAW_H

#include <stdio.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <glm/glm.hpp>
#include <map>

#include "shader.h"
#include "vertex_array.h"
#include "buffers.h"
#include "texture.h"

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

		Font(FT_Library *ft, const std::string name, int size);

		FT_Face face;
		std::map<char, Glyph> glyphs;
		std::string name;
		int size;
		std::unique_ptr<VertexArray>  VAO;
		std::unique_ptr<VertexBuffer> VBO;
	};

	// TODO : Use the resource manager to load fonts instead
	// TODO : Load hardcoded fonts 
	class FontManager {
	public:
		FontManager();
		~FontManager();

		Font* GetFont(std::string name, int size);
		Font* LoadFont(std::string name, int size);
		Font* GetDefaultFont() { return defaultFont; }
	private:
		std::vector<Font*> loadedFonts;
		Font* defaultFont;
		FT_Library ft;
	};

	/*
	 * Initializes shaders for later draw operations
	 */
	void DrawInit();
	void DrawInitRender();
	void DrawFinishRender();
	void DrawImg(float x, float y, std::shared_ptr<TextureBase> img, float width, float height, glm::vec4 color);
	
	void DrawFill(float x, float y, float width, float height, glm::vec4 color);
	
	void DrawString(float x, float y, std::string text, Font* font, float scale, glm::vec3 color, float alpha);
	
	/*
	 * Frees all shaders and data.
	 */
	void DrawFree();	
}

#endif
