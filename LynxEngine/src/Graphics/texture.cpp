#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "texture.h"
#include "Core/logger.h"
#include "rendererAPI.h"

namespace Lynx::Graphics {

	static int texcount = 0;

	Texture::Texture(int id, const char* path, const char* name) : BaseResource(id, path, name)
	{ 
		texture = RendererAPI::GenTexture();
		RendererAPI::BindTexture(texture);
		log_debug("Loading texture %s", path);

		int width, height, channels;
		unsigned char *data = stbi_load(path, &width, &height, &channels, 0);

		if(data){
			// Wrapping / Filtering settings
			switch ( IRendererAPI::GetAPI() ){
				case API_OPENGL:
					RendererAPI::SetTextureParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);	
					RendererAPI::SetTextureParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
					RendererAPI::SetTextureParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					RendererAPI::SetTextureParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					break;
			}
			
			RendererAPI::LoadTexture(data, width, height,false);
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
		RendererAPI::UseTexture(id);
		RendererAPI::BindTexture(texture);
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