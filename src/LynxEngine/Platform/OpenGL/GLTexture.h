#ifndef GLTEXTURE_H
#define GLTEXTURE_H

#include <vector>
#include <string>
#include <GL/glew.h>
#include "Graphics/texture.h"

namespace Lynx::Graphics::OpenGL {

    
    class GLTexture : public Lynx::Graphics::TextureBase {
    private:
        void loadDDSTex(std::string path);
        void freeDDSTex();
    public:
        GLTexture();
        GLTexture(std::string path);
        void Generate();
        void Use();
        void LoadFromFile(std::string path);
    };

    class GLCubemapTexture : public Lynx::Graphics::TextureBase {
    public:
        GLCubemapTexture();
        GLCubemapTexture(std::string basepath);
        void Use();
        void LoadFromFile(std::string path);
    };

}



#endif