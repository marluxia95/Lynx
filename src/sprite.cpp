#include <stdio.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include "sprite.h"
#include "vertexArray.h"
#include "vertexBuffer.h"

namespace Lynx {

vector<Vertex> sprite_vertices = { 
	//Position				 //Normal    // texture
	{vec3(0.0f, 0.0f, 0.0f), vec3(0.0f), vec3(0.0f, 1.0f, 0.0f)},
	{vec3(0.0f, 1.0f, 0.0f), vec3(0.0f), vec3(1.0f, 0.0f, 0.0f)},
	{vec3(1.0f, 0.0f, 0.0f), vec3(0.0f), vec3(0.0f)},
	{vec3(1.0f, 1.0f, 0.0f), vec3(0.0f), vec3(1.0f, 1.0f, 0.0f)}
};

vector<GLuint> sprite_indices = {
	0, 1, 2,
	1, 3, 2
};

Sprite::Sprite(Shader* shader):
    Mesh(&sprite_vertices, &sprite_indices, MESH_3D), shader(shader)
    {


}

Sprite::~Sprite(){
    shader->destroy();
}

void Sprite::Render(mat4 projection, mat4 view){
    VAO->Bind();

    if(!texture){
        texture->use();
    }

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(pos, 0.0f));
    shader->use();
    shader->setMat4("model", model);
    shader->setMat4("projection", projection);
    shader->setMat4("view", view);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDrawElements(GL_TRIANGLES, indices->size(), GL_UNSIGNED_INT, (void*)0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); 
}

}