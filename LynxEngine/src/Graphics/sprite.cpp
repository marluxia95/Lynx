#include <stdio.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Core/resourceManager.h"

#include "sprite.h"



extern Lynx::ResourceManager gResourceManager;

namespace Lynx::Graphics {

vector<Vertex> sprite_vertices = { 
	//Position				 //Normal    // texture
	{vec3(0.0f, 0.0f, 0.0f), vec3(0.0f), vec3(0.0f, 1.0f, 0.0f)},
	{vec3(0.0f, 1.0f, 0.0f), vec3(0.0f), vec3(1.0f, 0.0f, 0.0f)},
	{vec3(1.0f, 0.0f, 0.0f), vec3(0.0f), vec3(0.0f)},
	{vec3(1.0f, 1.0f, 0.0f), vec3(0.0f), vec3(1.0f, 1.0f, 0.0f)}
};

vector<GLuint> sprite_indices = {
	0, 1, 2,
	1, 3, 2
};

}
}