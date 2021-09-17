#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h> 
#include <stdio.h>
#include "Core/resourceManager.h"

namespace Lynx::Graphics {

	class Texture : public BaseResource {
	public:
		Texture(int id, const char* path, const char* name);
		void use();
		void Destroy();
		unsigned int texture = 0;
		int id;

	};

	typedef struct {
		int width, height, channels;
		unsigned char* data;
	} TextureData;

	TextureData* loadTexture(const char* path);
	void textureData_free(TextureData* data);

}

#endif

