#ifndef RENDERER_H
#define RENDERER_H


#include "shader.h"

class Renderer {
public:
	void draw(const unsigned int &VAO, const unsigned int &EBO, Shader &shader, int n);
	void draw(Shader &shader, int n);
	void draw(int n);
};


#endif