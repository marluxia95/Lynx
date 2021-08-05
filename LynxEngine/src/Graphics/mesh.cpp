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

#include "Platform/OpenGL/GLVertexArray.h"
#include "Platform/OpenGL/GLVertexBuffer.h"
#include "Platform/OpenGL/GLElementBuffer.h"

#include "Graphics/texture.h"
#include "Graphics/model.h"
#include "Graphics/mesh.h"
#include "Graphics/rendererAPI.h"

#include "Core/logger.h"


using namespace glm;
using namespace std;


namespace Lynx::Graphics {

Mesh::Mesh(vector<Vertex>* vertices, vector<GLuint>* indices, MeshType type ) 
	: vertices(vertices), indices(indices), type(type)
{	
	
	/* Create the vertex array object and the vertex buffer object				 
		TODO : Make this even more flexible by using only RendererAPI functions
	*/
	switch ( IRendererAPI::GetAPI() ) {
		case API_OPENGL: 
			VAO = std::make_unique<OpenGL::GLVertexArray>();
			VBO = std::make_unique<OpenGL::GLVertexBuffer>(&(vertices->at(0)), vertices->size() * sizeof(Vertex));
			break;
	}

	VAO->Bind();
	log_debug("Mesh type : %d", type);
	log_debug("Generated vertex buffer. Loaded %d vertices. Total VBO size : %d", vertices->size(), vertices->size() * sizeof(Vertex));

	// Configure the vertex buffer object
	VBO->Configure(type);

	// Create the element buffer object 
	switch ( IRendererAPI::GetAPI() ) {
		case API_OPENGL:
			EBO = std::make_unique<OpenGL::GLElementBuffer>(indices);
			break;
	}
	log_debug("Generated EBO with %d indices. Total EBO size : %d", indices->size(), indices->size() * sizeof(GLuint));
	VAO->Unbind();
}

void Mesh::Render()
{
		
}

void Mesh::Destroy()
{
	vertices->clear();
	indices->clear();
	delete vertices;
	delete indices;
	VAO->~VertexArray();
	VBO->~VertexBuffer(); 
	EBO->~ElementBuffer();
}

/*
	Converts mesh data into C++ code
*/
void meshToText(Mesh* mesh)
{
	printf("vector<Vertex> vertices = {");
	for(int v = 0; v < mesh->vertices->size(); v++){
		printf("\n{");
		printf("glm::vec3(%f, %f, %f),", mesh->vertices->at(v).Position.x, mesh->vertices->at(v).Position.y, mesh->vertices->at(v).Position.z);
		printf("glm::vec3(%f, %f, %f),", mesh->vertices->at(v).Normal.x, mesh->vertices->at(v).Normal.y, mesh->vertices->at(v).Normal.z);
		printf("glm::vec2(%f, %f)", mesh->vertices->at(v).TextureCoords.x, mesh->vertices->at(v).TextureCoords.y);
		if(v != mesh->vertices->size()-1)
			printf("},");
		else
			printf("}");
	}
	printf("};\n");
}

/*
	Converts mesh data into *pretty* C++ code
*/
void meshToTextPretty(Mesh* mesh)
{
	printf("vector<Vertex> vertices = {\n");
	for(int v = 0; v < mesh->vertices->size(); v++){
		printf("	{\n");
		printf("		glm::vec3(%f, %f, %f),\n", mesh->vertices->at(v).Position.x, mesh->vertices->at(v).Position.y, mesh->vertices->at(v).Position.z);
		printf("		glm::vec3(%f, %f, %f),\n", mesh->vertices->at(v).Normal.x, mesh->vertices->at(v).Normal.y, mesh->vertices->at(v).Normal.z);
		printf("		glm::vec2(%f, %f)\n", mesh->vertices->at(v).TextureCoords.x, mesh->vertices->at(v).TextureCoords.y);
		if(v != mesh->vertices->size()-1)
			printf("	},\n");
		else
			printf("	}\n");
	}
	printf("};\n");
}

}


