#ifndef GLTEXTURE_H
#define GLTEXTURE_H

#include <GL/glew.h> 
  
#include <stdio.h>

#include "Graphics/texture.h"

namespace Lynx::Graphics::OpenGL {

    class GLTexture : Lynx::Graphics::Texture {
    public:
        GLTexture(const char* path,int t_id, unsigned int texture_wrap_mode_s, unsigned int texture_wrap_mode_t,unsigned int texture_minifying_filter,unsigned int texture_magnifying_filter);
        GLTexture(const char* path,int t_id);
        void use();
        void Destroy();
        unsigned int texture = 0;
        int id;

    };

}

#endif

