#include <stdio.h>
#include <vector>
#include <glm/glm.hpp>

#include "Graphics/buffers.h"
#include "Graphics/vertex_array.h"
#include "Graphics/mesh.h"
#include "Core/logger.h"

#include "graphics_api.h"

using namespace glm;

namespace Lynx {

	Mesh::Mesh(std::string name, std::vector<Vertex>* vertices, std::vector<unsigned int>* indices, MeshType type )
		: vertices(vertices), indices(indices), type(type), Resource(name)
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
		API_CheckErrors();
	}

	void Mesh::Render()
	{
		// Indices mode
		EBO->Bind();
		//glDrawElements(GL_TRIANGLES, indices->size(), GL_UNSIGNED_INT, (void*)0);
		RendererAPI::DrawIndexed(indices->size());
	}

	Mesh::~Mesh()
	{
		delete vertices;
		delete indices;
	}

}


