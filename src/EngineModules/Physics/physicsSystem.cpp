#include "Core/application.h"
#include "physicsSystem.h"

namespace Lynx {

    void PhysicsSystem::Init()
    {

    }

    void PhysicsSystem::Update()
    {
        float dt = GameApplication::GetInstance()->GetDeltaTime();
        for(Lynx::EntityID ent : entities ) {
            PhysicsObject* phys_obj = scene->GetComponent<PhysicsObject>(ent);
            Transform* transform = scene->GetComponent<Transform>(ent);

            phys_obj->Force += phys_obj->Mass * gravity;

            phys_obj->Velocity += phys_obj->Force / phys_obj->Mass * dt;
            transform->position += phys_obj->Velocity;

            log_debug("P %d vx%f vy%f vz%f px%f py%f pz%f", ent, phys_obj->Velocity.x, phys_obj->Velocity.y, phys_obj->Velocity.z,
            transform->position.x, transform->position.y, transform->position.z);
            
            phys_obj->Force = glm::vec3(0.0f);
        }
    }

    void PhysicsSystem::OnEntityAdded(EntityID ent)
    {
        
    }

    void PhysicsSystem::OnEntityRemoved(EntityID ent)
    {

    }

}