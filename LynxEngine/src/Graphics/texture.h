#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdio.h>

namespace Lynx::Graphics {

    class Texture {
        public:
            Texture(const char* path,int t_id, unsigned int texture_wrap_mode_s, unsigned int texture_wrap_mode_t,unsigned int texture_minifying_filter,unsigned int texture_magnifying_filter);
            Texture(const char* path,int t_id);
            virtual void use() = 0;
            virtual void Destroy() = 0;
    };

}

#endif

