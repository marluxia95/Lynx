#ifndef LIGHTS_H
#define LIGHTS_H

#include <glm/glm.hpp>

struct point_light {
	glm::vec3 pos;
	glm::vec3 color;
	float intensity;
};

struct directional_light {
	glm::vec3 direction;
	glm::vec3 color;
	float intensity;
};

struct ambient_light {
	glm::vec3 color;
	float intensity;
};


#endif