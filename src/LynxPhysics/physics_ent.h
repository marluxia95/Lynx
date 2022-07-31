#ifndef PHYS_ENT_H
#define PHYS_ENT_H

#include "Core/entity.h"

namespace Lynx::Physics {

    /*
     *  Physics::PhysEnt : Physical entity, a normal entity, except it holds its physical properties. Meant to be managed and / or updated by
     *  Physics::World.
     */
    class PhysEnt : public Entity {
        public:
            PhysEnt() = default;

            float Mass = 1.0f;
            glm::vec3 Velocity = glm::vec3(0.0f);
            glm::vec3 Force = glm::vec3(0.0f);
    };

}

#endif // PHYS_ENT_H
