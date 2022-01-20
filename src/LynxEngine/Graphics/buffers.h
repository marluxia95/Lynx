#ifndef BUFFERS_H
#define BUFFERS_H

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "mesh.h"
#include "lynx_common.h"

using namespace glm;

namespace Lynx::Graphics {

	enum MeshType : unsigned int;

	struct Vertex {
		vec3 Position;
		vec3 Normal;
		vec2 TextureCoords;
	};

	void debugVBO(std::vector<Vertex>* vertices, std::vector<GLuint>* indices);

	class LYNXENGINE_API VertexBuffer{
	public:
		VertexBuffer();
		VertexBuffer(const void* data, unsigned int size);
        VertexBuffer(std::vector<Vertex>* vertices);
        VertexBuffer(std::vector<Vertex>* vertices, MeshType type);
        VertexBuffer(unsigned int size);
		~VertexBuffer();
		void SetData(const void* data, unsigned int size);
		void AddData(const void* data, unsigned int size);
		void Bind();
		void Unbind();
		void Configure(MeshType type);
		GLuint VBO_ID;
	};

	class LYNXENGINE_API ElementBuffer {
    public:
		ElementBuffer(const void* indices, unsigned int indexNumber);
        ElementBuffer(std::vector<unsigned int>* indices);
        ElementBuffer(unsigned int size);
        ElementBuffer();
		~ElementBuffer();
        void AddData(const void* data, unsigned int size);
        void Bind();
        void Unbind();
        GLuint ID;
    };

}

#endif