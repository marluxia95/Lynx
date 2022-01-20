#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H
#include <stdio.h>
#include "Core/ECS/system.h"
#include "Core/ECS/entity.h"
#include "Core/ECS/components.h"
#include "Graphics/cubemap.h"
#include "lynx_common.h"

namespace Lynx 
{

    
    class LYNXENGINE_API RenderSystem : public ECS::System 
    {
        public:
            RenderSystem(std::shared_ptr<Lynx::Scene> scene) : ECS::System(scene) {}
            void Init();
            void Update();
            void OnEntityAdded(EntityID ent) {}
            void OnEntityRemoved(EntityID ent) {}
            void SetCubemap(Graphics::Cubemap* cubemap);
            EntityID cameraEntity;
            EntityID directionalLight;
        private:
            Graphics::Cubemap* m_cubemap;
    };

}

#endif