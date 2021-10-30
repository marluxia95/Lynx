#ifndef LYNX_PHYSICS_SYSTEM_H
#define LYNX_PHYSICS_SYSTEM_H

#include <vector>
#include <glm/glm.hpp>
#include "Core/ECS/system.h"
#include "physicsObject.h"

namespace Lynx::Physics {
    
    class World {
        public:
            void Init();

            void Step(float dt);

            void AddObject(PhysicsObject* phys_obj) { m_objs.push_back(phys_obj); }

            void RemoveObject(PhysicsObject* phys_obj) { }

        private:
            std::vector<PhysicsObject*> m_objs;
            glm::vec3 gravity = glm::vec3(0, -9.8f, 0);

    };

    class System : public Lynx::ECS::System {
        public:
            System(Scene* scene) : ECS::System(scene) {}

            void Init();

            void Update();

            void OnEntityAdded(Entity ent);

            void OnEntityRemoved(Entity ent);

        private:
            World* phys_world;
    };
}




#endif // LYNX_PHYSICS_SYSTEM_H