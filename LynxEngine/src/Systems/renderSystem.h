#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <stdio.h>
#include <memory>

#include "Core/ECS/system.h"
#include "Core/ECS/entity.h"
#include "Core/ECS/components.h"
#include "lightingSystem.h"

namespace Lynx 
{
    typedef enum {
        RENDER_2D, RENDER_3D
    } RenderMode;
    
    class RenderSystem : public ECS::System 
    {
        public:
            void Init();
            void Update();

            void SetMainCamera(Entity cameraEnt);
            Entity GetMainCamera();

            void SetDirectionalLight(Entity dirLight);
            Entity GetDirectionalLight();

            void SetRenderMode(RenderMode mode);
            
        private:
            void render3D();
            void render2D();
            Entity cameraEntity;
            Entity directionalLight;

            RenderMode renderMode;
            std::shared_ptr<LightingSystem> lightingSystem;
    };

}

#endif