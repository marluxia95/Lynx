#ifndef LYNX_PHYSICS_SYSTEM_H
#define LYNX_PHYSICS_SYSTEM_H

#include <vector>
#include <glm/glm.hpp>
#include "Core/ECS/system.h"
#include "physicsObject.h"

namespace Lynx {

    class PhysicsSystem : public Lynx::ECS::System {
        public:
            PhysicsSystem(std::shared_ptr<Lynx::Scene> scene) : ECS::System(scene) {}

            void Init();

            void Update();

            void OnEntityAdded(EntityID ent);

            void OnEntityRemoved(EntityID ent);

        private:
            glm::vec3 gravity = glm::vec3(0, -9.8f, 0);
    };
}

#endif // LYNX_PHYSICS_SYSTEM_H