#ifndef GLRENDERERAPI_H
#define GLRENDERERAPI_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "Graphics/rendererAPI.h"

namespace Lynx::Graphics {

    class IRendererAPI;
    enum BufferType : unsigned int;

    namespace OpenGL {

        class GLRendererAPI : public Lynx::Graphics::IRendererAPI {
            public:
            void Init();

            void SetViewport(uint32_t width, uint32_t height);
            void Clear(glm::vec4 color);

            void LoadTexture(unsigned char* data, int width, int height, bool useDefaults=true);
            unsigned int GenTexture();
            void SetTextureParameter(unsigned int param, unsigned int value);
            void UseTexture(unsigned int id);
            void BindTexture(unsigned int tex);

            void GenBuffers(uint* bufferObj);
            void BindBuffer(Lynx::Graphics::BufferType buffer, uint* bufferobj);

            void RenderIndexed(int n);
        };

    }

}

#endif