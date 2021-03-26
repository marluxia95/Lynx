#ifndef MESH_H
#define MESH_H

#include <stdio.h>
#include <GL/glew.h> 
#include <glm/glm.hpp>
#include "vertexArray.h"
#include "vertexBuffer.h"
#include "elementBuffer.h"
#include "shader.h"

namespace Lynx {

	enum MeshType {
		MESH_2D_SPRITE,
		MESH_3D,
		MESH_3D_TEXTURED,
		MESH_3D_TEXTURED_NORMAL
	};

	class Mesh {

		public:
			Mesh(float* m_vertices, unsigned int m_numVertices, float* m_indices, unsigned int m_numIndices, MeshType type);
			~Mesh();
			void Render();

		private:
			VertexArray* VAO;
			VertexBuffer* VBO;
			ElementBuffer* EBO;
			float* vertices;
			float* indices;
			unsigned int numVertices;
			unsigned int numIndices;

			glm::mat4 model;

			MeshType type;



	};

	class Mesh3D : Mesh {
		public:
			Mesh3D(float* vertices, unsigned int numVertices, float* indices, unsigned int numIndices, Shader* shader, MeshType type);
			~Mesh3D();
		private:
			Shader* shader;
				
	};
}

#endif