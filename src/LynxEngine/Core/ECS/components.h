#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Graphics/shader.h"
#include "Graphics/mesh.h"
#include "Graphics/texture.h"
#include "Graphics/material.h"
#include "common.h"

using namespace glm;

namespace Lynx {

	class ComponentBase {};

	class Transform : public ComponentBase {
	public:
		Transform() : position(vec3(0)), rotation(vec3(0)), scale(vec3(0)) {}
		Transform(vec3 pos, vec3 rot, vec3 scale) : position(pos), rotation(rot), scale(scale) {}
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
			model = positionMatrix * scaleMatrix * rotationMatrix;

			return model;	
		}
	};

	class Generic : public ComponentBase {
	public:
		Generic(const char* name) : name(name) {}
		const char* name;
	};

	class Parent : public ComponentBase {
	public:
		EntityID parentEntity;
	};

	class MeshRenderer : public ComponentBase {
	public:
		MeshRenderer(std::shared_ptr<Graphics::Mesh> m) : mesh(m) {}
		vec3 ambient;
		vec3 diffuse;
		vec3 specular;
		Graphics::Material mat;
		std::shared_ptr<Graphics::Mesh> mesh;
		bool lighting = true;
	};

	class Light : public ComponentBase {
	public:
		Light() : ambient(vec3(1.0f)), diffuse(vec3(1.0f)), specular(vec3(1.0f)) {}
		Light(vec3 a, vec3 d, vec3 s) : ambient(a), diffuse(d), specular(s) {}
		vec3 ambient;
		vec3 diffuse;
		vec3 specular;
	};

	class PointLight : public Light {
	public:
		PointLight(vec3 a, vec3 d, vec3 s, float c, float l, float q) : Light(a, d, s), constant(c), linear(l), quadratic(q) {}
		PointLight(vec3 a, vec3 d, vec3 s) : Light(a, d, s), constant(), linear(), quadratic() {}
		PointLight(float c, float l, float q) : constant(c), linear(l), quadratic(q) {}
		float constant;
		float linear;
		float quadratic;
	};

	struct DirectionalLight : public Light {
	public:
		DirectionalLight(vec3 dir, float i) : direction(dir), intensity(i) {}
		DirectionalLight(vec3 a, vec3 d, vec3 s, vec3 dir, float i) : Light(a, d, s), direction(dir), intensity(i) {}
		glm::vec3 direction;
		float intensity;
	};

	struct Children {
		std::vector<EntityID> entityChildren;
	};

}

#endif
