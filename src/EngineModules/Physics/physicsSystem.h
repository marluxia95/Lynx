#ifndef LYNX_PHYSICS_SYSTEM_H
#define LYNX_PHYSICS_SYSTEM_H

#include <vector>
#include <glm/glm.hpp>
#include "Core/ECS/system.h"
#include "physicsObject.h"
#include "physicsWorld.h"

namespace Lynx {

    class PhysicsSystem : public Lynx::ECS::System {
        public:
            PhysicsSystem(std::shared_ptr<Lynx::Scene> scene) : ECS::System(scene) {}

            void Init();

            void Update();

            void OnEntityAdded(EntityID ent);

            void OnEntityRemoved(EntityID ent);

        private:
            Physics::World* phys_world;
    };
}




#endif // LYNX_PHYSICS_SYSTEM_H