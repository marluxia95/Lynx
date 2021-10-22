#include <stdio.h>
#include <glm/glm.hpp>
#include "physicsSystem.h"
#include "Core/application.h"
#include "Core/ECS/system.h"
#include "Core/ECS/components.h"

namespace Lynx {

	void PhysicsSystem::Init() 
	{
	}

	void PhysicsSystem::Update() 
	{
		float deltaTime = Lynx::GameApplication::GetInstance()->GetDeltaTime();
	
		for (auto entity : entities)
		{
			auto transform = scene->GetComponent<Transform>(entity);
			auto rigidbody = scene->GetComponent<RigidBody>(entity);

			transform->position += rigidbody->velocity * deltaTime;
			rigidbody->velocity += gravityForce * deltaTime;
		}
	}

}

