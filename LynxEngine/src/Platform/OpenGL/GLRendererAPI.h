#ifndef GLRENDERER_H
#define GLRENDERER_H

#include <stdio.h>
#include <stdint.h>
#include "Graphics/rendererAPI.h"

namespace Lynx::Graphics::OpenGL {

    class GLRendererAPI : RendererAPI {
        ~GLRendererAPI();

        virtual void Init();

        virtual void SetViewport(uint32_t width, uint32_t height);
        virtual void Clear(glm::vec4 color);

        virtual void RenderIndexed();
    };

}

#endif