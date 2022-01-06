#ifndef MESH_H
#define MESH_H

#include <stdio.h>
#include <iostream>
#include <vector>
#include <GL/glew.h> 
#include <glm/glm.hpp>
#include "vertexArray.h"
#include "buffers.h"
#include "lynx_common.h"
#include "Core/resource.h"

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

			VertexArray* VAO;
			VertexBuffer* VBO;
			ElementBuffer* EBO;
			MeshType type;
			bool success = true;
		protected:
			char* error;
	};

	//void meshToText(Mesh* mesh);
	//void meshToTextPretty(Mesh* mesh);
}

#endif
