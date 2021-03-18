#include <glad/glad.h>
#include "shader.h"

void Renderer::draw(const unsigned int &VAO, const unsigned int &EBO, Shader &shader){
	shader.use();
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}