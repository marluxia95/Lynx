#include <stdio.h>
#include <glm/glm.hpp>
#include "physicsSystem.h"

#include "Core/simpleGameEngine.h"

#include "Core/ECS/system.h"
#include "Core/ECS/components.h"

extern Lynx::Application gApplication;

namespace Lynx {

	void PhysicsSystem::Init() 
	{

	}

	void PhysicsSystem::Update() 
	{
		float deltaTime = gApplication.delta_time;

		for (auto entity : entities)
		{
			auto transform = gApplication.GetComponent<Transform>(entity);
			auto rigidbody = gApplication.GetComponent<RigidBody>(entity);

			transform->position += rigidbody->velocity * deltaTime;
			rigidbody->velocity += gravityForce * deltaTime;
		}
	}

}

