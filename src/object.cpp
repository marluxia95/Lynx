#include <stdio.h>
#include <string.h>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "object.h"
#include "shader.h"
#include "light.h"
#include "mesh.h"
#include "vertexBuffer.h"

using namespace std;

namespace Lynx {

Object3D::Object3D(vector<Vertex>* vertices, vector<GLuint>* indices, Shader* shader, Material material)
: Mesh3D(vertices, indices, shader, MESH_3D_NORMAL), shader(shader), material(material){

}

void Object3D::Render(glm::mat4 view, glm::mat4 projection,glm::vec3 viewPos , std::vector<PointLight*>* pointLights){
    VAO->Bind();
	model = mat4(1.0f);
	model = translate(model, this->pos);
	
	if(shader == nullptr) {error = "Shader is NULL!\n"; success = false; return;}
	// Set shader values
	shader->use();
	shader->setMat4("projection", projection);
	shader->setMat4("view", view);
	shader->setMat4("model", model);
	shader->setVec3("viewPos", viewPos);

	shader->setVec3("material.ambient", material.ambient);
	shader->setVec3("material.diffuse", material.diffuse);
	shader->setVec3("material.specular", material.specular);
	shader->setFloat("material.shininess", material.shininess);

    for (int l = 0; l<pointLights->size(); l++) { 

		string index = to_string(l);
        glUniform3fv(glGetUniformLocation(shader->ID, ("pointLights[" + index +"].position").c_str()),1, glm::value_ptr(pointLights->at(l)->pos));
        glUniform3fv(glGetUniformLocation(shader->ID, ("pointLights[" + index +"].ambient").c_str()),1, glm::value_ptr(pointLights->at(l)->ambient));
        glUniform3fv(glGetUniformLocation(shader->ID, ("pointLights[" + index +"].diffuse").c_str()),1, glm::value_ptr(pointLights->at(l)->diffuse));
        glUniform3fv(glGetUniformLocation(shader->ID, ("pointLights[" + index +"].specular").c_str()),1, glm::value_ptr(pointLights->at(l)->specular));
		
    }
	
	
	if(type >= MESH_3D_TEXTURED&&texture!=nullptr){
		if(texture->texture == NULL){
			error = "Model not bind to a texture !\n";
			success = false;
		}else{
			texture->use();
		}

	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDrawElements(GL_TRIANGLES, indices->size(), GL_UNSIGNED_INT, (void*)0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); 
}

}