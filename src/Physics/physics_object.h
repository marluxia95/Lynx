#ifndef PHYSICS_OBJECT_H
#define PHYSICS_OBJECT_H

#include <glm/glm.hpp>
#include "lynx_common.h"

namespace Lynx::Physics {

    class LYNXENGINE_API PhysicsObject {
    public:
        PhysicsObject();
        ~PhysicsObject();

        void Update();

        void SetPosition(glm::vec3 pos);
        glm::vec3 GetPosition();

        float GetMass();
        void SetMass(float mass=0.1f);

        void ApplyForce(glm::vec3 force);
        glm::vec3 GetForce();

        // Torque here ...

        void SetLinearVelocity(glm::vec3 lvel);
        glm::vec3 GetLinearVelocity();

        void SetGravity(glm::vec3 g);
        glm::vec3 GetGravity();
    private:
        float m_mass = 1.0f;
        glm::vec3 m_massCenter    = glm::vec3(0);
        glm::vec3 m_position      = glm::vec3(0);
        glm::vec3 m_force         = glm::vec3(0);
        glm::vec3 m_gravity       = glm::vec3(0, -0.1f, 0);
        glm::vec3 m_linearvel     = glm::vec3(0);
    };
}

#endif // PHYSICS_OBJECT_H
