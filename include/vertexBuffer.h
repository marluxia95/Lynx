#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>

using namespace std;
using namespace glm;

struct Vertex {
	vec3 Position;
	vec2 TextureCoords;
	vec3 Normal;
};

class VertexBuffer{
public:
	// "Classic" method
	VertexBuffer(const void* data, unsigned int size);
	VertexBuffer(vector<Vertex>* vertices);
	~VertexBuffer();
	void Bind();
	void Unbind();
private:
	unsigned int VBO_ID;
};

#endif