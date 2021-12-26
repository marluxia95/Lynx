#include "physicsWorld.h"

namespace Lynx::Physics {

    void World::Step(float dt)
    {
        for(PhysicsObject* phys_obj : m_objects) {
            phys_obj->Force += phys_obj->Mass * gravity;

            phys_obj->Velocity += phys_obj->Force / phys_obj->Mass;
            *phys_obj->Position += phys_obj->Velocity * dt;

            phys_obj->Force = glm::vec3(0.0f);
        }
    }

}