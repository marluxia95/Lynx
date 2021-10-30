#ifndef LYNX_PHYSICS_OBJECT_H
#define LYNX_PHYSICS_OBJECT_H

#include <glm/glm.hpp>

namespace Lynx::Physics {

    struct PhysicsObject {
        glm::vec3   Position;
        glm::vec3   Velocity;
        glm::vec3   Force;
        float       Mass;
    };

}

#endif // LYNX_PHYSICS_OBJECT_H