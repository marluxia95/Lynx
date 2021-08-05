#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "rendererAPI.h"

namespace Lynx::Graphics {

    class Texture {
        public:
            Texture(const char* path,int t_id, unsigned int texture_wrap_mode_s, unsigned int texture_wrap_mode_t,unsigned int texture_minifying_filter,unsigned int texture_magnifying_filter);
            Texture(const char* path,int t_id);
            void use();
            void Destroy();

        private:
            unsigned int tex;

    };

}

#endif

