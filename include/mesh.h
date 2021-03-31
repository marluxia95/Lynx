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
#include "model.h"

using namespace std;
using namespace glm;

namespace Lynx {

	enum MeshType {
		MESH_2D_SPRITE,
		MESH_3D,
		MESH_3D_TEXTURED,
		MESH_3D_TEXTURED_NORMAL
	};

	class Mesh {

		public:
			Mesh(vector<Vertex>* m_vertices, vector<GLuint>* m_indices, MeshType type);
			~Mesh();
			void Render();

		protected:
			VertexArray* VAO;
			GLuint VBO;
			GLuint EBO;

			vector<Vertex>* vertices;
			vector<GLuint>* indices;

			MeshType type;



	};

	class Mesh3D : Mesh {
		public:
			Mesh3D(vector<Vertex>* vertices, vector<GLuint>* indices, Shader* shader, MeshType type);
			Mesh3D(Model model, Shader* shader, MeshType type);
			~Mesh3D();
			void Render(glm::mat4 projection, glm::mat4 view);
			vec3 scale = vec3(1.0f);
			vec3 pos = vec3(0.0f);
			vec3 color = vec3(1.0f);
		protected:
			Shader* shader;
			mat4 model;
			
				
	};
}

#endif