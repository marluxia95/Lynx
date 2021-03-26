#ifndef SPRITE_H
#define SPRITE_H

#include <stdio.h>
#include <glm/glm.hpp>
#include "texture.h"
#include "shader.h"
#include "vertexArray.h"
#include "vertexBuffer.h"
#include "mesh.h"

namespace Lynx {
/*
class Sprite{
public:
	Sprite();
	//Sprite(const char* vertexPath;
	~Sprite();

	void Draw(glm::mat4 projection, glm::mat4 view);

	Texture* texture;
	glm::vec2 pos = glm::vec2(0.0f); 
	glm::vec2 size = glm::vec2(1.0f);
	glm::vec3 color = glm::vec3(1.0f);
	float ang;

private:

	Shader shader;
	VertexArray VAO;


	void init();

};
*/
class Sprite : Mesh {
	public:
	Sprite();
	~Sprite();

	void Draw(glm::mat4 projection, glm::mat4 view);

	Texture* texture;
	glm::vec2 pos =glm::vec2(0.0f);
	glm::vec2 size = glm::vec2(1.0f);
	glm::vec3 color = glm::vec3(1.0f);
	float ang;

	private:
		Shader shader;

};
}
#endif