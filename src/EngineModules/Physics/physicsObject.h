#ifndef LYNX_PHYSICS_OBJECT_H
#define LYNX_PHYSICS_OBJECT_H

#include <glm/glm.hpp>
#include "Core/ECS/entity.h"

namespace Lynx {

    struct PhysicsObject {
        PhysicsObject(float mass) : Mass(mass) {}

        glm::vec3   Velocity;
        glm::vec3   Force;
        float       Mass;
    };

}

#endif // LYNX_PHYSICS_OBJECT_H