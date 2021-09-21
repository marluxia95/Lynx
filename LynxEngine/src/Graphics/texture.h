#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h> 
#include <stdio.h>

namespace Lynx::Graphics {

	typedef struct {
		int width, height, channels;
		unsigned char* data;
	} TextureData;

	class Texture {
	public:
		Texture() : id(-1) {}
		Texture(const char* path);
		
		void LoadFromFile(const char* path);
		void Generate(int tid);

		bool IsValid() { return id != -1; }
		
		void use();

		unsigned int GetTextureID() { return texture; }
		int GetID() { return id; }
	private:
		unsigned int texture = 0;
		const char* path;
		int id;
		TextureData tex_data;
	};

	TextureData* loadTexture(const char* path);
	void textureData_free(TextureData* data);

}

#endif

