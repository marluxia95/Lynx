#include <stdio.h>
#include <GL/glew.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <gli/gli.hpp>

#include <chrono>
#include "texture.h"
#include "Core/logger.h"
#include "Utils/path.hpp"

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

	Texture::Texture() : TextureBase(TEXTURE_DEFAULT)
    {

    }

    Texture::Texture(std::string path) : TextureBase(TEXTURE_DEFAULT, path.c_str())
    {
        LoadFromFile(path);
    }

    void Texture::Generate()
    {
        glGenTextures(1,&texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        // Wrapping / Filtering settings
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if(data.data){
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, data.width, data.height, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data);
            glGenerateMipmap(GL_TEXTURE_2D);
            printf("Texture 1 loaded successfully\n");
        }else{
            printf("Unable to load texture\n");
        }

        id = TextureBase::PushTextureID();

        stbi_image_free(data.data);
    }

    void Texture::Use()
    {
        glActiveTexture(GL_TEXTURE0 + id);
        glBindTexture(GL_TEXTURE_2D, texture);
    }

    void Texture::LoadFromFile(std::string path)
    {
        std::string extension = Utils::GetFileExtension(path);

        if(extension == "dds" | extension == "ktx") {
            loadDDSTex(path);
        }else{
            loadSTBTex(path);
        }
    }

    void Texture::loadSTBTex(std::string path)
    {
        stbi_set_flip_vertically_on_load(true);  
	    data.data = stbi_load(path.c_str(), &data.width, &data.height, &data.channels, 0);
        Generate();
    }

    // From GLI examples
    void Texture::loadDDSTex(std::string path)
    {
        gli::texture Tex = gli::load(path);
        if(Tex.empty()) {
            log_error("Unable to open texture %s", path.c_str());
            return;
        }
        log_debug("Loading texture %s with format %d", path.c_str(), Tex.format());
        
        gli::gl GL(gli::gl::PROFILE_GL33);
        gli::gl::format const Format = GL.translate(Tex.format(), Tex.swizzles());
        GLenum Target = GL.translate(Tex.target());

        glGenTextures(1, &texture);
        glBindTexture(Target, texture);
        glTexParameteri(Target, GL_TEXTURE_BASE_LEVEL, 0);
        glTexParameteri(Target, GL_TEXTURE_MAX_LEVEL, static_cast<GLint>(Tex.levels() - 1));
        glTexParameteriv(Target, GL_TEXTURE_SWIZZLE_RGBA, &Format.Swizzles[0]);

        glm::tvec3<GLsizei> const Extent(Tex.extent());
	    GLsizei const FaceTotal = static_cast<GLsizei>(Tex.layers() * Tex.faces());
        
        switch(Tex.target())
        {
        case gli::TARGET_1D:
            glTexStorage1D(
                Target, static_cast<GLint>(Tex.levels()), Format.Internal, Extent.x);
            break;
        case gli::TARGET_1D_ARRAY:
        case gli::TARGET_2D:
        case gli::TARGET_CUBE:
            glTexStorage2D(
                Target, static_cast<GLint>(Tex.levels()), Format.Internal,
                Extent.x, Tex.target() == gli::TARGET_2D ? Extent.y : FaceTotal);
            break;
        case gli::TARGET_2D_ARRAY:
        case gli::TARGET_3D:
        case gli::TARGET_CUBE_ARRAY:
            glTexStorage3D(
                Target, static_cast<GLint>(Tex.levels()), Format.Internal,
                Extent.x, Extent.y,
                Tex.target() == gli::TARGET_3D ? Extent.z : FaceTotal);
            break;
        default:
            assert(0);
            break;
        }

        for(std::size_t Layer = 0; Layer < Tex.layers(); ++Layer)
        for(std::size_t Face = 0; Face < Tex.faces(); ++Face)
        for(std::size_t Level = 0; Level < Tex.levels(); ++Level)
        {
            GLsizei const LayerGL = static_cast<GLsizei>(Layer);
            glm::tvec3<GLsizei> Extent(Tex.extent(Level));
            Target = gli::is_target_cube(Tex.target())
                ? static_cast<GLenum>(GL_TEXTURE_CUBE_MAP_POSITIVE_X + Face)
                : Target;

            switch(Tex.target())
            {
            case gli::TARGET_1D:
                if(gli::is_compressed(Tex.format()))
                    glCompressedTexSubImage1D(
                        Target, static_cast<GLint>(Level), 0, Extent.x,
                        Format.Internal, static_cast<GLsizei>(Tex.size(Level)),
                        Tex.data(Layer, Face, Level));
                else
                    glTexSubImage1D(
                        Target, static_cast<GLint>(Level), 0, Extent.x,
                        Format.External, Format.Type,
                        Tex.data(Layer, Face, Level));
                break;
            case gli::TARGET_1D_ARRAY:
            case gli::TARGET_2D:
            case gli::TARGET_CUBE:
                if(gli::is_compressed(Tex.format()))
                    glCompressedTexSubImage2D(
                        Target, static_cast<GLint>(Level),
                        0, 0,
                        Extent.x,
                        Tex.target() == gli::TARGET_1D_ARRAY ? LayerGL : Extent.y,
                        Format.Internal, static_cast<GLsizei>(Tex.size(Level)),
                        Tex.data(Layer, Face, Level));
                else
                    glTexSubImage2D(
                        Target, static_cast<GLint>(Level),
                        0, 0,
                        Extent.x,
                        Tex.target() == gli::TARGET_1D_ARRAY ? LayerGL : Extent.y,
                        Format.External, Format.Type,
                        Tex.data(Layer, Face, Level));
                break;
            case gli::TARGET_2D_ARRAY:
            case gli::TARGET_3D:
            case gli::TARGET_CUBE_ARRAY:
                if(gli::is_compressed(Tex.format()))
                    glCompressedTexSubImage3D(
                        Target, static_cast<GLint>(Level),
                        0, 0, 0,
                        Extent.x, Extent.y,
                        Tex.target() == gli::TARGET_3D ? Extent.z : LayerGL,
                        Format.Internal, static_cast<GLsizei>(Tex.size(Level)),
                        Tex.data(Layer, Face, Level));
                else
                    glTexSubImage3D(
                        Target, static_cast<GLint>(Level),
                        0, 0, 0,
                        Extent.x, Extent.y,
                        Tex.target() == gli::TARGET_3D ? Extent.z : LayerGL,
                        Format.External, Format.Type,
                        Tex.data(Layer, Face, Level));
                break;
            default: assert(0); break;
            }
        }

        id = TextureBase::PushTextureID();
    }

}