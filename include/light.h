#ifndef LIGHTS_H
#define LIGHTS_H

#include <glm/glm.hpp>

typedef struct Material {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
}; 

typedef struct PointLight {
	glm::vec3 pos;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float constant;
    float linear;
    float quadratic;
};

typedef struct DirectionalLight {
	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float intensity;
};


#endif