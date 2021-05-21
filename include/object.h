#ifndef OBJECT_H
#define OBJECT_H

#include <stdio.h>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include "mesh.h"
#include "model.h"
#include "light.h"

namespace Lynx {

class Object3D : public Model{
    public:
        Object3D(vector<Vertex>* vertices, vector<GLuint>* indices, Shader* shader, Material material); 
        Object3D(const char* path, Shader* shader);
        ~Object3D();
        void Render(glm::mat4 view, glm::mat4 projection, glm::vec3 viewPos, std::vector<PointLight*>* pointLights);
        vec3 scale = vec3(1.0f);
		vec3 pos = vec3(0.0f);
		vec3 color = vec3(1.0f);
		Texture* texture = nullptr;
        char* error;
        bool success = true;
    protected:
        mat4 model;
        Shader* shader;
        Material material;
        const aiScene *scene;
};

}
#endif