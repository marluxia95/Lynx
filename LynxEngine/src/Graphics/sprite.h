#ifndef SPRITE_H
#define SPRITE_H

#include <stdio.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>

#include "shader.h"

#include "mesh.h"

using namespace std;
using namespace glm;

namespace Lynx::Graphics {

vector<Vertex> sprite_vertices;


/* 

class SpriteMesh : public Mesh{
	public:
		Sprite();
		~Sprite();
	
		void Render(mat4 projection, mat4 view);
};
}

*/

#endif