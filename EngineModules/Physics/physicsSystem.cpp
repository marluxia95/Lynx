#include "Core/application.h"
#include "physicsSystem.h"

namespace Lynx {

    void PhysicsSystem::Init()
    {
        phys_world = new Physics::World();
    }

    void PhysicsSystem::Update()
    {
        phys_world->Step(GameApplication::GetInstance()->GetDeltaTime());
    }

    void PhysicsSystem::OnEntityAdded(Entity ent)
    {
        phys_world->AddObject(scene->GetComponent<PhysicsObject>(ent));
        
    }

    void PhysicsSystem::OnEntityRemoved(Entity ent)
    {

    }

}