#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "texture.h"
#include "shader.h"
#include "entity.h"
#include "mesh.h"
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

struct Parent {
	Entity parentEntity;
};

struct MeshRenderer {
	vec3 color;
	Mesh* mesh;
	Shader* shader;
	Texture* texture;
};

struct RigidBody {
	vec3 velocity;
	vec3 acceleration;
	float mass;
};

struct Material {
    	glm::vec3 ambient;
    	glm::vec3 diffuse;
    	glm::vec3 specular;
    	float shininess;
}; 

struct PointLight {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float constant;
   	float linear;
    	float quadratic;
};

struct DirectionalLight {
	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float intensity;
};

}

#endif
