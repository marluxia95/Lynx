#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <iostream>
#include <stdio.h>
#include <map>
#include "Graphics/mesh.h"
#include "Graphics/shader.h"
#include "Graphics/texture.h"
#include "Graphics/vertexBuffer.h"
#include "logger.h"

namespace Lynx {

class ResourceManager{
	public:
		ResourceManager();
		~ResourceManager();
		void clear();

		Shader* LoadShader(const char* vertexFile, const char* fragmentFile, const char* Name);
		Shader* GetShader(const char* name);

		Texture* LoadTexture(const char* file, const char* name);
		Texture* GetTexture(const char* name);

		Mesh* LoadMesh(const char* name, vector<Vertex>* vertices, vector<GLuint>* indices, MeshType type);
		Mesh* GetMesh(const char* name);

		std::map<const char*, Shader*> Shaders;
		std::map<const char*, Texture*> Textures;
		std::map<const char*, Mesh*> Meshes;
 
};

}


#endif