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
            PhysicsSystem(Scene* scene) : ECS::System(scene) {}

            void Init();

            void Update();

            void OnEntityAdded(Entity ent);

            void OnEntityRemoved(Entity ent);

        private:
            Physics::World* phys_world;
    };
}




#endif // LYNX_PHYSICS_SYSTEM_H