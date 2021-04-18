#include <stdio.h>
#include <vector>
#include <string.h>

#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "vertexBuffer.h"
#include "model.h"
#include "vertexArray.h"
#include "texture.h"

using namespace std;

namespace Lynx {

Model::Model(const char* path, Shader* shader): path(path), shader(shader){
    loadModel();
}

void Model::Render(){
    for (int i = 0; i<meshes.size(); i++)
        meshes[i].Render();

    
}

void Model::loadModel(){
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);	
	
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        printf("Error while loading model %s \n", importer.GetErrorString());
        return;
    }

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene){
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; 
        meshes.push_back(processMesh(mesh, scene));			
    }

    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene){
    vector<Vertex>* vertices;
    vector<GLuint>* indices;
    vector<Texture>* textures;

    // Process vertices
    for(unsigned int i = 0; i < mesh->mNumVertices; i++) 
    {
        Vertex vertex;
        glm::vec3 vector; 
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z; 
        vertex.Position = vector;
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.Normal = vector; 
        if(mesh->mTextureCoords[0]){
            glm::vec2 texCoord;
            texCoord.x = mesh->mTextureCoords[0][i].x;
            texCoord.y = mesh->mTextureCoords[0][i].y;
            vertex.TextureCoords = texCoord;
        }

        vertices->push_back(vertex);
    }

    // Process indices
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices->push_back(face.mIndices[j]);
    }  


    return Mesh(vertices, indices, MESH_3D_TEXTURED_NORMAL);
}

}