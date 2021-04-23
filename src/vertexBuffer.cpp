#include <GL/glew.h> 	
#include <iostream>
#include <vector>
#include "vertexBuffer.h"

using namespace std;

VertexBuffer::VertexBuffer(const void* data, unsigned int size){
	glGenBuffers(1, &VBO_ID);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);  
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::VertexBuffer(vector<Vertex>* vertices){
	glGenBuffers(1, &VBO_ID);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);  
	glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(Vertex), &vertices->at(0), GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer(){
	glDeleteBuffers(1, &VBO_ID);
}

void VertexBuffer::Bind(){
	glBindBuffer(GL_ARRAY_BUFFER, VBO_ID); 
}

void VertexBuffer::Unbind(){
	glBindBuffer(GL_ARRAY_BUFFER, 0); 
}

void debugVBO(vector<Vertex>* vertices, vector<GLuint>* indices){
	for ( int i = 0; i < indices->size(); i++ ){
		printf("Index Number %d Vertex pos: %f %f %f Texture Coord pos %f %f Normal pos %f %f %f\n", i, vertices->at(indices->at(i)).Position.x, 
		vertices->at(indices->at(i)).Position.y, 
		vertices->at(indices->at(i)).Position.z,
		vertices->at(indices->at(i)).TextureCoords.x,
		vertices->at(indices->at(i)).TextureCoords.y,
		vertices->at(indices->at(i)).Normal.x,
		vertices->at(indices->at(i)).Normal.y,
		vertices->at(indices->at(i)).Normal.z);
	}
}