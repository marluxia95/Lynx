#include <iostream>
#include <stdio.h>
#include <map>
#include "resourceManager.h"
#include "logger.h"

namespace Lynx {

ResourceManager::~ResourceManager() 
{
	clear();
}

void ResourceManager::clear() 
{
	log_debug("Cleaning resources");

	for(auto const& rs : Shaders){
		delete rs.second;
	}

	for(auto const& rm : Meshes){
		delete rm.second;
	}

	for(auto const& rt : Textures){
		delete rt.second;
	}
}

Graphics::Texture* ResourceManager::LoadTexture(const char* name, const char* path)
{
	if(Textures.find(name) != Textures.end())
		return Textures[name];

	Textures[name] = new Graphics::Texture(path);
	return Textures[name];
}

Graphics::Shader* ResourceManager::LoadShader(const char* name, const char* file)
{
	if(Shaders.find(name) != Shaders.end())
		return Shaders[name];
	
	Shaders[name] = new Graphics::Shader(file);
	return Shaders[name];
}

Graphics::Mesh* ResourceManager::LoadMesh(const char* name, vector<Graphics::Vertex>* vertices, vector<unsigned int>* indices, Graphics::MeshType type)
{
	if(Meshes.find(name) != Meshes.end())
		return Meshes[name];

	Meshes[name] = new Graphics::Mesh(vertices, indices, type);
	return Meshes[name];
}

Graphics::Texture* ResourceManager::GetTexture(const char* name)
{
	if(Textures.find(name) == Textures.end()){
		log_error("Texture resource %s does not exist!", name);
		return NULL;
	}
	return Textures[name];
}

Graphics::Shader* ResourceManager::GetShader(const char* name)
{
	if(Shaders.find(name) == Shaders.end()){
		log_error("Shader resource %s does not exist!", name);
		return NULL;
	}
	return Shaders[name];
}

Graphics::Mesh* ResourceManager::GetMesh(const char* name)
{
	if(Meshes.find(name) == Meshes.end()){
		log_error("Mesh resource %s does not exist!", name);
		return NULL;
	}
	return Meshes[name];
}

}