#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>

using namespace std;
using namespace glm;

namespace Lynx {

	struct Vertex {
		vec3 Position;
		vec3 Normal;
		vec2 TextureCoords;
	};

	void debugVBO(vector<Vertex>* vertices, vector<GLuint>* indices);

	class VertexBuffer{
	public:
		// "Classic" method
		VertexBuffer(const void* data, unsigned int size);
		VertexBuffer(vector<Vertex>* vertices);
		~VertexBuffer();
		void Bind();
		void Unbind();
		unsigned int VBO_ID;
	};

}

#endif