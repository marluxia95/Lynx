#include <stdio.h>
#include <iostream>
#include <vector>
#include <GL/glew.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Graphics/vertexArray.h"
#include "Graphics/vertexBuffer.h"
#include "Graphics/elementBuffer.h"
#include "Graphics/texture.h"
#include "Graphics/model.h"
#include "Graphics/mesh.h"

#include "Core/logger.h"


using namespace glm;
using namespace std;


namespace Lynx {

Mesh::Mesh(vector<Vertex>* vertices, vector<GLuint>* indices, MeshType type ) 
	: vertices(vertices), indices(indices), type(type)
{
	renderType = RENDER_INDEXED;

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);  
	glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(Vertex), &(vertices->at(0)), GL_STATIC_DRAW);
	
	log_debug("Mesh type : %d", type);
	log_debug("Generated vertex buffer. Loaded %d vertices. Total VBO size : %d", vertices->size(), vertices->size() * sizeof(Vertex));

	VAO = new VertexArray();
	VAO->Bind();
	

	// VAO Configuration

	switch( type ){

		case MESH_2D_SPRITE:
			// Vertex attribute
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    		glEnableVertexAttribArray(0);
			//printf("Mesh type : MESH_2D_SPRITE\n");
			break;
		case MESH_3D:
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
			glEnableVertexAttribArray(0);
			//printf("Mesh type : MESH_3D\n");
			break;
		case MESH_3D_NORMAL:
			// Vertex attribute
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
			glEnableVertexAttribArray(0);
			// Normal attribute
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
			glEnableVertexAttribArray(1);
			//printf("Mesh type : MESH_3D_NORMAL\n");
			break;
		case MESH_3D_TEXTURED:
			// Vertex attribute
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    		glEnableVertexAttribArray(0);
    		// Texture attribute
    		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TextureCoords));
    		glEnableVertexAttribArray(1);
			//printf("Mesh type : MESH_3D_TEXTURED\n");
			break;
		case MESH_3D_TEXTURED_NORMAL:
			// Vertex attribute
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
			glEnableVertexAttribArray(0);
			// Texture attribute
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TextureCoords));
			glEnableVertexAttribArray(1);
			// Normal attribute
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
			glEnableVertexAttribArray(2);
			//printf("Mesh type : MESH_3D_TEXTURED_NORMAL\n");
			break;
	}

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices->size() * sizeof(GLuint), &(indices->at(0)), GL_STATIC_DRAW );
	VAO->Unbind();
	log_debug("Generated EBO with %d indices. Total EBO size : %d", indices->size(), indices->size() * sizeof(GLuint));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); 

	
    


}

Mesh::Mesh(){}

Mesh::~Mesh()
{

}

void Mesh::Render()
{
	// Indices mode
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDrawElements(GL_TRIANGLES, indices->size(), GL_UNSIGNED_INT, (void*)0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); 	
}

void Mesh::Destroy()
{
	vertices->clear();
	indices->clear();
}

void meshToText(Mesh* mesh)
{
	printf("vector<Vertex> vertices = {\n");
	for(int v = 0; v < mesh->vertices->size(); v++){
		printf("{\n");
		printf("	glm::vec3(%f, %f, %f),\n");
		printf("	glm::vec3(%f, %f, %f),\n");
		printf("	glm::vec3(%f, %f, %f)\n");
		if(v != mesh->vertices->size()-1)
			printf("},\n");
		else
			printf("}\n");
	}
	printf("};\n");
}

}


