#include <stdio.h>
#include <GL/glew.h> 
#include <glm/glm.hpp>
#include "vertexArray.h"
#include "vertexBuffer.h"
#include "elementBuffer.h"
#include "mesh.h"

namespace Lynx {

Mesh::Mesh(float* m_vertices, unsigned int m_numVertices, float* m_indices, unsigned int m_numIndices, MeshType type){
	vertices = m_vertices;
	numVertices = m_numVertices;
	indices = m_indices;
	numIndices = m_numIndices;
	type = type;


	VBO = new VertexBuffer(vertices, numVertices);

	VAO->Bind();

	// VAO Configuration

	switch( type ){

		case MESH_2D_SPRITE:
			// Vertex attribute
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    		glEnableVertexAttribArray(0);
			break;
		case MESH_3D:
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			break;
		case MESH_3D_TEXTURED:
			// Vertex attribute
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    		glEnableVertexAttribArray(0);
    		// Texture attribute
    		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
    		glEnableVertexAttribArray(1);
			break;
		case MESH_3D_TEXTURED_NORMAL:
			// Vertex attribute
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			// Texture attribute
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3*sizeof(float)));
			glEnableVertexAttribArray(1);
			// Normal attribute
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3*sizeof(float)));
			glEnableVertexAttribArray(2);
			break;
	}

	VAO->Unbind();

	EBO = new ElementBuffer(&indices, numIndices);

}

Mesh::~Mesh(){

}

void Mesh::Render(){

	
	model = glm::mat4(1.0f);
	
	VAO->Bind();
	glDrawElements(GL_TRIANGLES, numVertices, GL_UNSIGNED_INT, 0);
	VAO->Unbind();
}


Mesh3D::Mesh3D(float* vertices, unsigned int numVertices, float* indices, unsigned int numIndices, Shader* shader, MeshType type)
	: Mesh(vertices, numVertices, indices, numIndices, type){
	if(type<MESH_3D){printf("Invalid mesh type\n"); return;}
	
}


}


