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
#include "model.h"

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
			Mesh(); // does nothing
			~Mesh();
			MeshType GetType();
			void Render();
			char* getError();
			bool checkErrors();
			vector<Vertex>* vertices;
			vector<GLuint>* indices;
			

		protected:
			VertexArray* VAO;
			GLuint VBO;
			GLuint EBO;
			bool success = true;
			char* error;
			
			RenderType renderType;
			MeshType type;



	};

	class Mesh3D : public Mesh {
		public:
			Mesh3D(vector<Vertex>* vertices, vector<GLuint>* indices, Shader* shader, MeshType type); // 3D mesh using indices
			Mesh3D(const char* path, Shader* shader, MeshType type); // model mode						 
			~Mesh3D();
			void Render(glm::mat4 projection, glm::mat4 view);
			void Render(void(*renderfunction)());
			vec3 scale = vec3(1.0f);
			vec3 pos = vec3(0.0f);
			vec3 color = vec3(1.0f);
			Texture* texture = nullptr;
		protected:
			void loadModel();
			void generateMesh();
			Shader* shader;
			mat4 model;
			MeshType type;
			
			
				
	};
}

#endif