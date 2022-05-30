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
            transform->position += phys_obj->Velocity * dt;
            
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