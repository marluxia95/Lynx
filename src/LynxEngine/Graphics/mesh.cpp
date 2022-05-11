#include <stdio.h>
#include <vector>
#include <glm/glm.hpp>

#include "Graphics/buffers.h"
#include "Graphics/vertexArray.h"
#include "Graphics/mesh.h"
#include "Core/logger.h"
#include "debug.h"

#include "rendererAPI.h"

using namespace glm;

namespace Lynx::Graphics {

	Mesh::Mesh(std::string name, std::vector<Vertex>* vertices, std::vector<unsigned int>* indices, MeshType type )
		: vertices(vertices), indices(indices), type(type), ResourceBase(name)
	{
		
		VBO = VertexBuffer::Create(vertices);

		log_debug("Mesh type : %d", type);
		log_debug("Generated vertex buffer. Loaded %d vertices. Total VBO size : %d", vertices->size(), vertices->size() * sizeof(Vertex));

		VAO = VertexArray::Create();
		VAO->Bind();
		
		VBO->Configure(type);

		EBO = ElementBuffer::Create(indices);

		VAO->Unbind();
		log_debug("Generated EBO with %d indices. Total EBO size : %d", indices->size(), indices->size() * sizeof(unsigned int));
		glCheckError();
	}

	void Mesh::Render()
	{
		// Indices mode
		EBO->Bind();
		glDrawElements(GL_TRIANGLES, indices->size(), GL_UNSIGNED_INT, (void*)0);
	}

	Mesh::~Mesh()
	{
		delete vertices;
		delete indices;
		vertices->push_back(Vertex{});
	}

}


