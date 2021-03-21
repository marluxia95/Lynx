#include <stdio.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include "sprite.h"
#include "vertexArray.h"
#include "vertexBuffer.h"

namespace Lynx {

Sprite::Sprite():
	shader("res/shaders/generic/sprite.vs", "res/shaders/generic/sprite.fs")
{
	printf("Sprite init\n");
	init();

}

Sprite::~Sprite(){

}

void Sprite::init(){

	float vertices[] = { 
        // pos      // texture
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 
    
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    printf("Generating vertex buffer \n");

	VertexBuffer VBO(vertices, sizeof(vertices));

	VAO.Bind();

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    VBO.Unbind();
    VAO.Unbind();

    printf("Generated VAO & VBO for sprite\n");


}

void Sprite::Draw(glm::mat4 projection){
    if(texture == nullptr | texture == NULL){printf("Sprite not bind to a texture !\n");}
	shader.use();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(pos, 0.0f));

	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); 
    model = glm::rotate(model, glm::radians(ang), glm::vec3(0.0f, 0.0f, 1.0f)); 
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

    model = glm::scale(model, glm::vec3(size, 1.0f)); 

    shader.setMat4("model", model);
    shader.setMat4("projection", projection);
    shader.setVec3("spriteColor", color);

    texture->use();

    VAO.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    VAO.Unbind();

}

}