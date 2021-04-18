#ifndef MODEL_H
#define MODEL_H
#include <stdio.h>
#include <vector>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "shader.h"
#include "mesh.h"

using namespace std;

namespace Lynx {

class Model {
    public:
        Model(const char* path, Shader* shader);
        void Render(glm::mat4 projection, glm::mat4 view);
        const char* name;
        glm::vec3 pos;
    private:
        vector<Mesh3D> meshes;
        const char * path;
        const char* directory;
        Shader* shader;

        void loadModel();
        void processNode(aiNode* node, const aiScene* scene);
        Mesh3D processMesh(aiMesh* mesh, const aiScene* scene);
};

}


#endif