#include "physics_object.h"
#include "Core/application.h"

namespace Lynx::Physics {

    PhysicsObject::PhysicsObject()
    {

    }

    PhysicsObject::~PhysicsObject()
    {

    }

    void PhysicsObject::Update()
    {
        float deltatime = Application::GetSingleton()->GetDeltaTime();

        m_position += m_linearvel;

        ApplyForce(m_gravity);

        if(glm::length(m_force) == 0)
            return;

        m_linearvel += m_force / m_mass * deltatime;
        m_force = glm::vec3(0);
    }

    void PhysicsObject::SetPosition(glm::vec3 pos)
    {
        m_position = pos;
    }

    glm::vec3 PhysicsObject::GetPosition()
    {
        return m_position;
    }

    float PhysicsObject::GetMass()
    {
        return m_mass;
    }

    void PhysicsObject::SetMass(float mass)
    {
        m_mass = mass;
    }

    void PhysicsObject::ApplyForce(glm::vec3 force)
    {
        m_force = force;
    }

    glm::vec3 PhysicsObject::GetForce()
    {
        return m_force;
    }

    void PhysicsObject::SetLinearVelocity(glm::vec3 lvel)
    {
        m_linearvel = lvel;
    }

    glm::vec3 PhysicsObject::GetLinearVelocity()
    {
        return m_linearvel;
    }

    void PhysicsObject::SetGravity(glm::vec3 g)
    {
        m_gravity = g;
    }

    glm::vec3 PhysicsObject::GetGravity()
    {
        return m_gravity;
    }

}
