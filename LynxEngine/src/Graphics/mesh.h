#ifndef MESH_H
#define MESH_H

#include <stdio.h>
#include <iostream>
#include <vector>
#include <memory>
#include <GL/glew.h> 
#include <glm/glm.hpp>

#include "vertexArray.h"
#include "vertexBuffer.h"
#include "elementBuffer.h"

#include "shader.h"

using namespace std;
using namespace glm;

namespace Lynx::Graphics {

	struct Vertex;
	class VertexBuffer;
	class ElementBuffer;

	enum MeshType : unsigned int {
		MESH_2D,
		MESH_2D_SPRITE,
		MESH_3D,
		MESH_3D_NORMAL,
		MESH_3D_TEXTURED,
		MESH_3D_TEXTURED_NORMAL
	};

	class Mesh {

		public:
			Mesh(vector<Vertex>* vertices, vector<GLuint>* indices, MeshType type); // Element buffer mode
			void Destroy();
			void Render();
			vector<Vertex>* vertices;
			vector<GLuint>* indices;

			std::unique_ptr<VertexArray> VAO;
			std::unique_ptr<VertexBuffer> VBO;
			std::unique_ptr<ElementBuffer> EBO;
			MeshType type;
			bool success = true;
		protected:
			char* error;
	};

	void meshToText(Mesh* mesh);
	void meshToTextPretty(Mesh* mesh);
}

#endif