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

namespace Lynx {
	
	struct Glyph
	{
		// Advance
		float ax;
		float ay;

		// Bitmap
		float bw;
		float bh;
		float bl;
		float bt;

		// Offset
		float tx;
		float ty;
	};

	
	struct Font
	{

		Font(FT_Library *ft, const std::string name, int size);

		unsigned int tex;
		FT_Face face;
		Glyph c[128];
		std::string name;
		int size = 0;
	        unsigned int w = 0;
	        unsigned int h = 0;
		int attrib = -1;
	        std::unique_ptr<VertexBuffer> VBO;
		std::unique_ptr<VertexArray> VAO;
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
	void DrawTex(float x, float y, unsigned int texture_id, float width, float height, glm::vec4 color);
	void DrawFill(float x, float y, float width, float height, glm::vec4 color);
	
	void DrawString(float x, float y, const char* text, Font* font, float scale, glm::vec3 color, float alpha);
	
	/*
	 * Frees all shaders and data.
	 */
	void DrawFree();	
}

#endif
