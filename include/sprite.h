#ifndef SPRITE_H
#define SPRITE_H

#include <stdio.h>
#include <glm/glm.hpp>
#include "texture.h"
#include "shader.h"
#include "vertexArray.h"
#include "vertexBuffer.h"

namespace Lynx {

class Sprite{
public:
	Sprite();
	//Sprite(const char* vertexPath;
	~Sprite();

	void Draw(glm::mat4 projection);

	Texture* texture;
	glm::vec2 pos; 
	glm::vec2 size;
	glm::vec3 color;
	float ang;

private:

	Shader shader;
	VertexArray VAO;


	void init();

};

}
#endif