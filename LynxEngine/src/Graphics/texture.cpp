#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <chrono>
#include "texture.h"
#include "Core/logger.h"
#include "rendererAPI.h"

namespace Lynx::Graphics {

	int TextureBase::total_textures = 0;

	void TextureData::Free()
	{
		stbi_image_free(data);
	}

	TextureBase::TextureBase(TextureType type) : id(-1), type(type)
	{ 
		log_debug("Created new texture");
		data = new TextureData(this); 
	}

	TextureBase::TextureBase(TextureType type, const char* path) : id(-1), type(type)
	{ 
		log_debug("Created new texture with path %s", path);
		data = new TextureData(this); 
		data->path = path;
	}

	Texture::Texture(const char* path) : TextureBase(TEXTURE_DEFAULT, path)
	{ 
		
	}

	void Texture::Use()
	{
		if(!IsValid())
			return;
		
		RendererAPI::UseTexture(id);
		RendererAPI::BindTexture(texture);
	}

	void TextureBase::loadFromFile(const char* path)
	{
		log_debug("Loading texture %s", path);
		data->path = path;
		data->data = stbi_load(path, &data->width, &data->height, &data->channels, 0);
		if(!data)
			log_error("Unable to load texture %s : %s", path, stbi_failure_reason());
	}

	void Texture::Generate()
	{
		log_debug("Before generating texture");
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
			RendererAPI::LoadTexture(data->data, data->width, data->height, false);
			log_debug("Texture data dump : Path=%s, Data len=%d, W=%d, H=%d", data->GetPath(), sizeof(data->data), data->width, data->height);
			id = TextureBase::PushTextureID();
			log_debug("After generating texture");
		}else{
			log_error("Unable to load texture %s", data->path);
		}
		API_CheckErrors();
		data->Free();
	}


}