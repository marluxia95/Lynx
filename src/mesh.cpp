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
#include "vertexArray.h"
#include "vertexBuffer.h"
#include "elementBuffer.h"
#include "texture.h"
#include "model.h"
#include "mesh.h"


using namespace glm;
using namespace std;


namespace Lynx {

Mesh::Mesh(vector<Vertex>* vertices, vector<GLuint>* indices, MeshType type ) : vertices(vertices), indices(indices), type(type){
	renderType = RENDER_INDEXED;

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);  
	glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(Vertex), &(vertices->at(0)), GL_STATIC_DRAW);
	//printf("Generated vertex buffer. Loaded %d vertices. Total VBO size : %d\n", vertices->size(), vertices->size() * sizeof(Vertex));

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
	//printf("Generated EBO with %d indices. Total EBO size : %d\n", indices->size(), indices->size() * sizeof(GLuint));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); 

	
    


}

MeshType Mesh::GetType(){
	return type;
}

Mesh::Mesh(){}

bool Mesh::checkErrors(){
	return success;
}

Mesh::~Mesh(){

}

void Mesh::Render(){
	// Indices mode
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDrawElements(GL_TRIANGLES, indices->size(), GL_UNSIGNED_INT, (void*)0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); 	
}


MeshRenderer::MeshRenderer(vector<Vertex>* vertices, vector<GLuint>* indices, Shader* shader, MeshType type)
	: Mesh(vertices, indices, type), shader(shader){
	if(type<MESH_3D){error = "Invalid mesh type\n"; success = false; return;}

}

MeshRenderer::MeshRenderer(const char* path, Shader* shader, MeshType type)
	: Mesh(vertices, indices, type), shader(shader){
	if(type<MESH_3D){error = "Invalid mesh type\n"; success = false; return;}
}

MeshRenderer::~MeshRenderer(){
	shader->destroy();
}

void MeshRenderer::Render(mat4 projection, mat4 view){/*
	VAO->Bind();
	model = mat4(1.0f);
	//model = translate(model, this->pos);
	
	if(shader == nullptr) {error = "Shader is NULL!\n"; success = false; return;}
	// Set shader values
	shader->use();
	shader->setMat4("projection", projection);
	shader->setMat4("view", view);
	shader->setMat4("model", model);
	
	if(type >= MESH_3D_TEXTURED){
		if(texture == nullptr){
			error = "Model not bind to a texture !\n";
			success = false;
		}else if(texture->texture != NULL){
			texture->use();
		}

	}
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDrawElements(GL_TRIANGLES, indices->size(), GL_UNSIGNED_INT, (void*)0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); */
}

}


