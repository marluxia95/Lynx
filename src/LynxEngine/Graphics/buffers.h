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
		static VertexBuffer* Create(std::vector<Vertex>* vertices);
		static VertexBuffer* Create(std::vector<Vertex>* vertices, MeshType type);
	};

	class LYNXENGINE_API ElementBuffer {
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
        static ElementBuffer* Create(std::vector<GLuint>* indices);
    };

}

#endif