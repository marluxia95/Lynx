#ifndef BUFFERS_H
#define BUFFERS_H

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "mesh.h"

using namespace std;
using namespace glm;

namespace Lynx::Graphics {

	enum MeshType : unsigned int;

	struct Vertex {
		vec3 Position;
		vec3 Normal;
		vec2 TextureCoords;
	};

	void debugVBO(vector<Vertex>* vertices, vector<GLuint>* indices);

	class VertexBuffer{
	public:
		VertexBuffer() = default;
		~VertexBuffer() = default;
		virtual void SetData(const void* data, unsigned int size) = 0;
		virtual void AddData(const void* data, unsigned int size) = 0;
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual void Configure(MeshType type) = 0;
		unsigned int VBO_ID;

		static VertexBuffer* Create();
		static VertexBuffer* Create(const void* data, unsigned int size);
		static VertexBuffer* Create(unsigned int size);
		static VertexBuffer* Create(vector<Vertex>* vertices);
		static VertexBuffer* Create(vector<Vertex>* vertices, MeshType type);
	};

	class ElementBuffer {
    public:
		ElementBuffer() = default;
		~ElementBuffer() = default;
        virtual void AddData(const void* data, unsigned int size) = 0;
        virtual void Bind() = 0;
        virtual void Unbind() = 0;
        unsigned int ID;

		static ElementBuffer* Create();
		static ElementBuffer* Create(unsigned int size);
		static ElementBuffer* Create(const void* indices, unsigned int indexNumber);
        static ElementBuffer* Create(vector<GLuint>* indices);
    };

}

#endif