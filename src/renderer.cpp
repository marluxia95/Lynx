#include <glad/glad.h>
#include "renderer.h"
#include "shader.h"

void Renderer::draw(const unsigned int &VAO, const unsigned int &EBO, Shader &shader, int n){
	shader.use();
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, n, GL_UNSIGNED_INT, 0);
}

void Renderer::draw(Shader &shader, int n){
	shader.use();
	glDrawArrays(GL_TRIANGLES, 0, n);
}

void Renderer::draw(int n){
	glDrawArrays(GL_TRIANGLES, 0, n);
}