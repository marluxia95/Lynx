#ifndef MESH_H
#define MESH_H

#include <stdio.h>
#include <iostream>
#include <vector>
#include <GL/glew.h> 
#include <glm/glm.hpp>
#include "vertexArray.h"
#include "buffers.h"

using namespace std;
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

	class Mesh {

		public:
			Mesh(vector<Vertex>* vertices, vector<unsigned int>* indices, MeshType type); // Element buffer mode
			Mesh() { };
			~Mesh();
			virtual void Render();
			vector<Vertex>* vertices;
			vector<unsigned int>* indices;

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
