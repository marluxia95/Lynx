#include <stdio.h>
#include <glad/glad.h>
#include "vertexArray.h"

VertexArray::VertexArray(){
	glGenVertexArrays(1, &VAO_ID);
	glBindVertexArray(VAO_ID);


}

VertexArray::~VertexArray(){
	glDeleteVertexArrays(1, &VAO_ID);
}

void VertexArray::Bind(){
	glBindVertexArray(VAO_ID);
}

void VertexArray::Unbind(){
	glBindVertexArray(0);
}