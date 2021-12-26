#ifndef RENDERER2D_H
#define RENDERER2D_H

#include <glm/glm.hpp>
#include "vertexArray.h"
#include "buffers.h"
#include "Core/ECS/components.h"
#include "lynx_common.h"

namespace Lynx::Graphics {

    class LYNXENGINE_API Renderer2D {
        public:
            static void Init();
            static void RenderQuad();

            static void Destroy();
    };

}


#endif