#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H
#include <stdio.h>


#include "ECS/system.h"
#include "ECS/entity.h"
#include "ECS/components.h"

namespace Lynx 
{

    
    class RenderSystem : public ECS::System 
    {
        public:
            void Init();
            void Update();
            Entity cameraEntity;
            Entity directionalLight;
            
    };

}

#endif