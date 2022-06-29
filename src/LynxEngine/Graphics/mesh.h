#ifndef MESH_H
#define MESH_H

#include <stdio.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>

#include "Core/resource.h"
#include "vertex_array.h"
#include "buffers.h"
#include "lynx_common.h"

using namespace glm;

namespace Lynx::Graphics {

	class VertexBuffer;
	class ElementBuffer;
	struct Vertex;

	enum MeshType : unsigned int {
		MESH_2D_SPRITE,
		MESH_3D,
		MESH_3D_NORMAL,
		MESH_3D_TEXTURED,
		MESH_3D_TEXTURED_NORMAL
	};

	enum RenderType : unsigned int {
		RENDER_ARRAYS,
		RENDER_INDEXED
	};

	class LYNXENGINE_API Mesh : public ResourceBase {
		public:
			Mesh(std::string name, std::vector<Vertex>* vertices, std::vector<unsigned int>* indices, MeshType type); // Element buffer mode
			Mesh(std::string name) : ResourceBase(name) { };
			Mesh() : ResourceBase() { };
			~Mesh();
			virtual void Render();
			std::vector<Vertex>* vertices;
			std::vector<unsigned int>* indices;

			std::unique_ptr<VertexArray> VAO;
			std::unique_ptr<VertexBuffer> VBO;
			std::unique_ptr<ElementBuffer> EBO;
			MeshType type;
			bool success = true;
		protected:
			char* error;
	};

	
}

#endif
