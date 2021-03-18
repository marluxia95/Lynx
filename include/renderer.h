#ifndef RENDERER_H
#define RENDERER_H


#include "shader.h"

class Renderer {
public:
	void draw(const unsigned int &VAO, const unsigned int &EBO, Shader &shader);
}


#endif