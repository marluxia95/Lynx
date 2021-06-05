#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <stdio.h>
#include <glm/glm.hpp>
#include "mesh.h"
#include "shader.h"

using namespace glm;

namespace Lynx {

struct Transform {
	vec3 position;
	quat rotation;
	vec3 scale;
};

struct GameObject {
	const char* name;
};

struct MeshRenderer {
	vec3 color;
	Mesh* mesh;
	Shader* shader;
};

struct RigidBody {
	vec3 velocity;
	vec3 acceleration;
	float mass;
};

}

#endif