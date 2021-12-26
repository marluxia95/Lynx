#ifndef LYNX_PHYSICS_WORLD
#define LYNX_PHYSICS_WORLD

#include <vector>
#include "physicsObject.h"

namespace Lynx::Physics {

    class World {
        public:
            void Init();

            void Step(float dt);

            void AddObject(PhysicsObject* phys_obj) { m_objects.push_back(phys_obj); }

            void RemoveObject(PhysicsObject* phys_obj) { }

        private:
            std::vector<PhysicsObject*> m_objects;
            glm::vec3 gravity = glm::vec3(0, -9.8f, 0);

    };   
}

#endif // LYNX_PHYSICS_WORLD