#include <stdio.h>
#include <iostream>
#include <vector>
#include <GL/glew.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "vertexArray.h"
#include "vertexBuffer.h"
#include "elementBuffer.h"
#include "mesh.h"

using namespace glm;
using namespace std;

namespace Lynx {

Mesh::Mesh(vector<Vertex>* vertices, vector<GLuint>* indices, MeshType type ) : vertices(vertices), indices(indices), type(type){

	VAO = new VertexArray();
	VBO = new VertexBuffer(vertices, vertices->size());
	EBO = new ElementBuffer(indices);
	
	VAO->Bind();
	EBO->Bind();
	

	// VAO Configuration

	switch( type ){

		case MESH_2D_SPRITE:
			// Vertex attribute
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    		glEnableVertexAttribArray(0);
			break;
		case MESH_3D:
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
			glEnableVertexAttribArray(0);
			break;
		case MESH_3D_TEXTURED:
			// Vertex attribute
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    		glEnableVertexAttribArray(0);
    		// Texture attribute
    		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TextureCoords));
    		glEnableVertexAttribArray(1);
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
			break;
	}

	VAO->Unbind();

	

}

Mesh::~Mesh(){

}

void Mesh::Render(){
	VAO->Bind();
	glDrawElements(GL_TRIANGLES, vertices->size(), GL_UNSIGNED_INT, 0);
	VAO->Unbind();
}


Mesh3D::Mesh3D(vector<Vertex>* vertices, vector<GLuint>* indices, Shader* shader, MeshType type)
	: Mesh(vertices, indices, type){
	if(type<MESH_3D){printf("Invalid mesh type\n"); return;}
	if(shader == nullptr) {
		shader = new Shader("res/shaders/standard/textured_box.vs", "res/shaders/standard/textured_box.fs");
	}else{
		shader = shader;
	}

}

void Mesh3D::Render(mat4 projection, mat4 view){
	model = mat4(1.0f);
	model = translate(model, pos);
	
	// Set model values
	/*
	shader->setMat4("projection", projection);
	shader->setMat4("model", model);
	shader->setMat4("view", view);
	*/
	printf("%d\n", this->vertices->size());
	VAO->Bind();
	VBO->Bind();
	glDrawElements(GL_TRIANGLES, this->vertices->size(), GL_UNSIGNED_INT, 0);
	VAO->Unbind();
}

}


