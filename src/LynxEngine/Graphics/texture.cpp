#include <stdio.h>
#include <GL/glew.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "Platform/OpenGL/GLRendererAPI.h"

#include <chrono>
#include "texture.h"
#include "Core/logger.h"
#include "Utils/path.hpp"

#define TEXTURE_DEFAULT TEXTURE_2D

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

    Texture::Texture(TextureType type) : TextureBase(type)
    {

    }

    Texture::Texture(std::string path, TextureType type) : TextureBase(type, path.c_str())
    {
        LoadFromFile(path);
    }

    void Texture::Generate()
    {
        if(using_gli) {
            genDDSTex();
            return;
        }

        texture = RendererAPI::GenTexture();

        if(!data.data){
            printf("Unable to load texture\n");
            stbi_image_free(data.data);
            return;
        }
        
        RendererAPI::LoadTexture(TEXTURE_2D, data.data, data.width, data.height);
        id = TextureBase::PushTextureID();
        printf("Texture %d loaded successfully\n", id);
        stbi_image_free(data.data);
    }

    void Texture::Use()
    {
        RendererAPI::BindTexture(type, texture);
    }

    void Texture::LoadFromFile(std::string path)
    {
        std::string extension = Utils::GetFileExtension(path);
        m_resPath = path;

        if(extension == "dds" | extension == "ktx") {
            tex = gli::load(path);
            using_gli = true;

            log_debug("Loaded texture %s into memory", path.c_str());

            if(tex.empty()) {
                log_error("Unable to open texture %s", path.c_str());
                return;
            }
        }else{
            loadSTBTex(path);
        }


    }

    void Texture::loadSTBTex(std::string path)
    {
        stbi_set_flip_vertically_on_load(true);  
	    data.data = stbi_load(path.c_str(), &data.width, &data.height, &data.channels, 0);
    }

    // From GLI examples
    void Texture::genDDSTex()
    {
        if(tex.empty()) {
            log_error("Invalid texture");
            return;
        }
        
        log_debug("Loading texture with format %d", tex.format());
        
        gli::gl GL(gli::gl::PROFILE_GL33);
        gli::gl::format const Format = GL.translate(tex.format(), tex.swizzles());
        GLenum Target = GL.translate(tex.target());

        glGenTextures(1, &texture);
        glBindTexture(Target, texture);
        glTexParameteri(Target, GL_TEXTURE_BASE_LEVEL, 0);
        glTexParameteri(Target, GL_TEXTURE_MAX_LEVEL, static_cast<GLint>(tex.levels() - 1));
        glTexParameteriv(Target, GL_TEXTURE_SWIZZLE_RGBA, &Format.Swizzles[0]);
   
        API_CheckErrors();

        glm::tvec3<GLsizei> const Extent(tex.extent());
	    GLsizei const FaceTotal = static_cast<GLsizei>(tex.layers() * tex.faces());

        switch(tex.target())
        {
        case gli::TARGET_1D:
            glTexStorage1D(
                Target, static_cast<GLint>(tex.levels()), Format.Internal, Extent.x);
            break;
        case gli::TARGET_1D_ARRAY:
        case gli::TARGET_2D:
        case gli::TARGET_CUBE:
            glTexStorage2D(
                Target, static_cast<GLint>(tex.levels()), Format.Internal,
                Extent.x, Extent.y );
            break;
        case gli::TARGET_2D_ARRAY:
        case gli::TARGET_3D:
        case gli::TARGET_CUBE_ARRAY:
            glTexStorage3D(
                Target, static_cast<GLint>(tex.levels()), Format.Internal,
                Extent.x, Extent.y,
                tex.target() == gli::TARGET_3D ? Extent.z : FaceTotal);
            break;
        default:
            assert(0);
            break;
        }

        API_CheckErrors();

        for(std::size_t Layer = 0; Layer < tex.layers(); ++Layer)
        for(std::size_t Face = 0; Face < tex.faces(); ++Face)
        for(std::size_t Level = 0; Level < tex.levels(); ++Level)
        {
            GLsizei const LayerGL = static_cast<GLsizei>(Layer);
            glm::tvec3<GLsizei> Extent(tex.extent(Level));
            log_debug("IsCubeMap? %s", gli::is_target_cube(tex.target()) ? "yes" : "no");
            Target = gli::is_target_cube(tex.target())
                ? GL_TEXTURE_CUBE_MAP_POSITIVE_X + Face
                : Target;

            switch(tex.target())
            {
            case gli::TARGET_1D:
                if(gli::is_compressed(tex.format()))
                    glCompressedTexSubImage1D(
                        Target, static_cast<GLint>(Level), 0, Extent.x,
                        Format.Internal, static_cast<GLsizei>(tex.size(Level)),
                        tex.data(Layer, Face, Level));
                else
                    glTexSubImage1D(
                        Target, static_cast<GLint>(Level), 0, Extent.x,
                        Format.External, Format.Type,
                        tex.data(Layer, Face, Level));
                break;
            case gli::TARGET_1D_ARRAY:
            case gli::TARGET_2D:
            case gli::TARGET_CUBE:
                if(gli::is_compressed(tex.format()))
                    glCompressedTexSubImage2D(
                        Target, static_cast<GLint>(Level),
                        0, 0,
                        Extent.x,
                        tex.target() == gli::TARGET_1D_ARRAY ? LayerGL : Extent.y,
                        Format.Internal, static_cast<GLsizei>(tex.size(Level)),
                        tex.data(Layer, Face, Level));
                else
                    glTexSubImage2D(
                        Target, static_cast<GLint>(Level),
                        0, 0,
                        Extent.x,
                        tex.target() == gli::TARGET_1D_ARRAY ? LayerGL : Extent.y,
                        Format.External, Format.Type,
                        tex.data(Layer, Face, Level));
                break;
            case gli::TARGET_2D_ARRAY:
            case gli::TARGET_3D:
            case gli::TARGET_CUBE_ARRAY:
                if(gli::is_compressed(tex.format()))
                    glCompressedTexSubImage3D(
                        Target, static_cast<GLint>(Level),
                        0, 0, 0,
                        Extent.x, Extent.y,
                        tex.target() == gli::TARGET_3D ? Extent.z : LayerGL,
                        Format.Internal, static_cast<GLsizei>(tex.size(Level)),
                        tex.data(Layer, Face, Level));
                else
                    glTexSubImage3D(
                        Target, static_cast<GLint>(Level),
                        0, 0, 0,
                        Extent.x, Extent.y,
                        tex.target() == gli::TARGET_3D ? Extent.z : LayerGL,
                        Format.External, Format.Type,
                        tex.data(Layer, Face, Level));
                break;
            default: assert(0); break;
            }
        }

        API_CheckErrors();

        id = TextureBase::PushTextureID();
    }

}