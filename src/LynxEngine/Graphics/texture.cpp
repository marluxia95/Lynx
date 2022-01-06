#include <stdio.h>

#include <chrono>
#include "texture.h"
#include "Core/logger.h"
#include "rendererAPI.h"
#include "Platform/OpenGL/GLTexture.h"

namespace Lynx::Graphics {

	int TextureBase::total_textures = 0;

	TextureBase::TextureBase(TextureType type) : id(-1), type(type), ResourceBase()
	{ 
		log_debug("Created new texture");
	}

	TextureBase::TextureBase(TextureType type, const char* path) : id(-1), type(type), ResourceBase(path)
	{ 
		log_debug("Creating new texture with path %s", path);
	}

	std::shared_ptr<TextureBase> TextureBase::CreateTexture(std::string path)
	{
		switch( IRendererAPI::GetAPI() ) {
			case API_OPENGL:
				return std::make_shared<OpenGL::GLTexture>(path);
		}
	}

}