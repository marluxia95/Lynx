#include <stdio.h>
#include <GL/glew.h> 

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "texture.h"

#include "Core/logger.h"

namespace Lynx {

	static int texcount = 0;

	Texture::Texture(const char* path) 
	{ 
		glGenTextures(1,&texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		log_debug("Loading texture %s", path);

		int width, height, channels;
		unsigned char *data = stbi_load(path, &width, &height, &channels, 0);

		if(data){
			// Wrapping / Filtering settings
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			id = texcount++;
			log_debug("Texture id is %d", id);
		}else{
			log_error("Unable to load texture %s", path);
		}

		stbi_image_free(data);
	}

	void Texture::Destroy()
	{

	}

	void Texture::use()
	{
		glActiveTexture(GL_TEXTURE0 + id);
		glBindTexture(GL_TEXTURE_2D, texture);
	}

	TextureData* loadTexture(const char* path)
	{
		TextureData* texdata = (TextureData*)malloc(sizeof(TextureData));

		int width, height, channels;
		unsigned char *data = stbi_load(path, &width, &height, &channels, 0);

		if(!data){
			log_error("Unable to load texture %s", path);
		}
		texdata->width = width;
		texdata->height = height;
		texdata->channels = channels;
		texdata->data = data;

		return texdata;
	}

	void textureData_free(TextureData* data)
	{
		stbi_image_free(data->data);
		free(data);
	}

}