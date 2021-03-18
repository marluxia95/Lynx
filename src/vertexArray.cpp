#include <stdio.h>
#include <glad/glad.h>

VertexArray::VertexArray(float* vertices){
	glGenVertexArrays(1, &VAO_ID);
	glBindVertexArray(VAO_ID);


}

void VertexArray::Bind(){
	glBindVertexArray(VAO_ID);
}

void VertexArray::Unbind(){
	glBindVertexArray(0);
}