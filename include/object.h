#ifndef OBJECT_H
#define OBJECT_H

#include <stdio.h>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include "mesh.h"
#include "light.h"

namespace Lynx {

class Object3D : public Mesh3D {
    public:
        Object3D(vector<Vertex>* vertices, vector<GLuint>* indices, Shader* shader, Material material); 
        Object3D(const char* path, Shader* shader);
        ~Object3D();
        void Render(glm::mat4 view, glm::mat4 projection, glm::vec3 viewPos, std::vector<PointLight*>* pointLights);
    protected:
        Shader* shader;
        Material material;
        const aiScene *scene;
};

}
#endif