#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <iostream>
#include <stdio.h>
#include <map>
#include "Graphics/mesh.h"
#include "Graphics/shader.h"
#include "Graphics/texture.h"
#include "Graphics/buffers.h"
#include "application.h"

namespace Lynx {

class Application;

class Resource {
	public:
		int id;
		const char* path;

};

class ResourceManager{
	public:
		ResourceManager(Application* application) : application(application) {}
		~ResourceManager();
		void clear();

		Graphics::Shader* LoadShader(const char* name, const char* file);
		Graphics::Shader* GetShader(const char* name);

		Graphics::Texture* LoadTexture(const char* name, const char* file);
		Graphics::Texture* GetTexture(const char* name);

		Graphics::Mesh* LoadMesh(const char* name, vector<Graphics::Vertex>* vertices, vector<unsigned int>* indices, Graphics::MeshType type);
		Graphics::Mesh* GetMesh(const char* name);

		std::map<const char*, Graphics::Shader*> Shaders;
		std::map<const char*, Graphics::Texture*> Textures;
		std::map<const char*, Graphics::Mesh*> Meshes;

	private:
		Application* application; // Application reference
 
};

}


#endif