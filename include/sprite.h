#ifndef SPRITE_H
#define SPRITE_H

#include <stdio.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include "texture.h"
#include "shader.h"
#include "vertexArray.h"
#include "vertexBuffer.h"
#include "mesh.h"

using namespace std;
using namespace glm;

namespace Lynx {

class Sprite : Mesh {
	public:
		Sprite(Shader* shader);
		~Sprite();

		void Render(mat4 projection, mat4 view);

		const char* name;
		Texture* texture;
		vec2 pos =vec2(0.0f);
		vec2 size = vec2(1.0f);
		vec3 color = vec3(1.0f);
		float ang;

	private:
		Shader* shader;


	

};
}
#endif