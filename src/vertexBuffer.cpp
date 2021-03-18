#include <glad/glad.h>
#include "vertexBuffer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size){
	glGenBuffers(1, &VBO_ID);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);  
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
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