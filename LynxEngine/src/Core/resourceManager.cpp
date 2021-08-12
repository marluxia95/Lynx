#include <iostream>
#include <stdio.h>
#include <map>
#include "resourceManager.h"
#include "Graphics/shader.h"
#include "Graphics/mesh.h"
#include "logger.h"


namespace Lynx {


ResourceManager::ResourceManager() 
{

	log_log(LOG_INFO, "Resource manager started");

}

ResourceManager::~ResourceManager() 
{
	clear();
}

void ResourceManager::clear() 
{
	log_debug("Cleaning resources");

	for(auto const& rs : Shaders){
		rs.second->destroy();
		delete rs.second;
	}

	for(auto const& rm : Meshes){
		rm.second->Destroy();
		delete rm.second;
	}

	for(auto const& rt : Textures){
		rt.second->Destroy();
		delete rt.second;
	}
}

Texture* ResourceManager::LoadTexture(const char* name, const char* path)
{
	if(Textures.find(name) != Textures.end())
		return Textures[name];

	Textures[name] = new Texture(path);
	return Textures[name];
}

Shader* ResourceManager::LoadShader(const char* name, const char* vertexPath, const char* fragmentPath)
{
	if(Shaders.find(name) != Shaders.end())
		return Shaders[name];
	
	Shaders[name] = new Shader(vertexPath, fragmentPath);
	return Shaders[name];
}

Mesh* ResourceManager::LoadMesh(const char* name, vector<Vertex>* vertices, vector<GLuint>* indices, MeshType type)
{
	if(Meshes.find(name) != Meshes.end())
		return Meshes[name];

	Meshes[name] = new Mesh(vertices, indices, type);
	return Meshes[name];
}

Texture* ResourceManager::GetTexture(const char* name)
{
	if(Textures.find(name) == Textures.end()){
		log_error("Texture resource %s does not exist!", name);
		return NULL;
	}
	return Textures[name];
}

Shader* ResourceManager::GetShader(const char* name)
{
	if(Shaders.find(name) == Shaders.end()){
		log_error("Shader resource %s does not exist!", name);
		return NULL;
	}
	return Shaders[name];
}

Mesh* ResourceManager::GetMesh(const char* name)
{
	if(Meshes.find(name) == Meshes.end()){
		log_error("Mesh resource %s does not exist!", name);
		return NULL;
	}
	return Meshes[name];
}

}