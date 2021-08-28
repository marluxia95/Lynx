#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <iostream>
#include <stdio.h>
#include <map>
#include "Graphics/mesh.h"
#include "Graphics/shader.h"
#include "Graphics/texture.h"
#include "Graphics/buffers.h"

namespace Lynx {

class ResourceManager{
	public:
		ResourceManager();
		~ResourceManager();
		void clear();

		Graphics::Shader* LoadShader(const char* vertexFile, const char* fragmentFile, const char* Name);
		Graphics::Shader* GetShader(const char* name);

		Graphics::Texture* LoadTexture(const char* file, const char* name);
		Graphics::Texture* GetTexture(const char* name);

		Graphics::Mesh* LoadMesh(const char* name, vector<Graphics::Vertex>* vertices, vector<unsigned int>* indices, Graphics::MeshType type);
		Graphics::Mesh* GetMesh(const char* name);

		std::map<const char*, Graphics::Shader*> Shaders;
		std::map<const char*, Graphics::Texture*> Textures;
		std::map<const char*, Graphics::Mesh*> Meshes;
 
};

}


#endif