/**
 * @file transformSystem.h
 * @author marlxx (marlxx@protonmail.com)
 * @brief Manages and updates all the matrices from the transform components
 * @date 2022-06-14
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef TRANSFORMSYSTEM_H
#define TRANSFORMSYSTEM_H

#include "Core/ECS/system.h"
#include "Core/ECS/entity.h"
#include "lynx_common.h"

namespace Lynx 
{
    class LYNXENGINE_API TransformSystem : public ECS::System {
    public:
        TransformSystem(std::shared_ptr<Lynx::Scene> scene) : ECS::System(scene) {}
        void Init();
        void Update();
        void OnEntityAdded(EntityID ent) {}
        void OnEntityRemoved(EntityID ent) {}
    private:
        void updateMatrices(Entity ent);
    };

}
#endif