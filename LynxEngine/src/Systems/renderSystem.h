#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H
#include <stdio.h>


#include "Core/ECS/system.h"
#include "Core/ECS/entity.h"
#include "Core/ECS/components.h"
#include "Graphics/cubemap.h"

namespace Lynx 
{

    
    class RenderSystem : public ECS::System 
    {
        public:
            void Init();
            void Update();
            void SetCubemap(Cubemap* cubemap);
            Entity cameraEntity;
            Entity directionalLight;
        private:
            Cubemap* m_cubemap;
    };

}

#endif