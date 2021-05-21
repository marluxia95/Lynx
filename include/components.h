#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <stdio.h>
#include <glm/glm.hpp>

using namespace glm;

struct Transform {
	vec3 position;
	quat rotation;
	vec3 scale;
};

struct RigidBody {
	vec3 mass;
};

#endif