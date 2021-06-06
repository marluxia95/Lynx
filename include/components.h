#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "mesh.h"
#include "shader.h"

using namespace glm;

namespace Lynx {

struct Transform {
	vec3 position;
	vec3 rotation;
	vec3 scale;

	mat4 GetModel()
	{
		mat4 model = mat4(1.0f);

		mat4 positionMatrix = glm::translate(model, position);
		mat4 scaleMatrix = glm::scale(model, scale);
		mat4 rotationMatrix_x = glm::rotate(model, glm::radians(rotation.x), vec3(1.0f, 0.0f, 0.0f));
		mat4 rotationMatrix_y = glm::rotate(model, glm::radians(rotation.y), vec3(0.0f, 1.0f, 0.0f));
		mat4 rotationMatrix_z = glm::rotate(model, glm::radians(rotation.z), vec3(0.0f, 0.0f, 1.0f));
		mat4 rotationMatrix = rotationMatrix_x * rotationMatrix_y * rotationMatrix_z;
		model = positionMatrix * rotationMatrix * scaleMatrix;

		return model;	
	}
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