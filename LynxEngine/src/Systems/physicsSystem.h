#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include <stdio.h>
#include <glm/glm.hpp>
#include "Core/ECS/system.h"

using namespace glm;

namespace Lynx {

class PhysicsSystem : public ECS::System {
	
	public:
		PhysicsSystem(Scene* scene) : ECS::System(scene) {}
		void Init();
		void Update();
		vec3 gravityForce = vec3(0, -3.0f,0);
};

}

#endif