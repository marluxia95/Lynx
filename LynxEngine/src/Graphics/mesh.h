#ifndef MESH_H
#define MESH_H

#include <stdio.h>
#include <iostream>
#include <vector>
#include <GL/glew.h> 
#include <glm/glm.hpp>
#include "vertexArray.h"
#include "vertexBuffer.h"
#include "elementBuffer.h"
#include "shader.h"
#include "texture.h"

using namespace std;
using namespace glm;

namespace Lynx {

	enum MeshType {
		MESH_2D_SPRITE,
		MESH_3D,
		MESH_3D_NORMAL,
		MESH_3D_TEXTURED,
		MESH_3D_TEXTURED_NORMAL
	};

	enum RenderType {
		RENDER_ARRAYS,
		RENDER_INDEXED
	};

	class Mesh {

		public:
			Mesh(vector<Vertex>* vertices, vector<GLuint>* indices, MeshType type); // Element buffer mode
			Mesh();
			void Destroy();
			void Render();
			vector<Vertex>* vertices;
			vector<GLuint>* indices;

			VertexArray* VAO;
			GLuint VBO;
			GLuint EBO;
			MeshType type;
			bool success = true;
		protected:
			char* error;
	};

	void meshToText(Mesh* mesh);
	void meshToTextPretty(Mesh* mesh);
}

#endif
