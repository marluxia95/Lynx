#include "texture.h"

namespace Lynx::Graphics {

    Texture::Texture(const char* path,int t_id, unsigned int texture_wrap_mode_s, unsigned int texture_wrap_mode_t,unsigned int texture_minifying_filter,unsigned int texture_magnifying_filter)
    {
        tex = RendererAPI::GenTexture();
        RendererAPI::BindTexture(tex);
        int width, height, channels;
        unsigned char *data = stbi_load(path, &width, &height, &channels, 0);
        RendererAPI::LoadTexture(data, width, height, false);
        stbi_image_free(data);
    }

    Texture::Texture(const char* path, int t_id)
    {
        tex = RendererAPI::GenTexture();
        RendererAPI::BindTexture(tex);
        int width, height, channels;
        unsigned char *data = stbi_load(path, &width, &height, &channels, 0);
        RendererAPI::LoadTexture(data, width, height, true);
        stbi_image_free(data);
    }



}