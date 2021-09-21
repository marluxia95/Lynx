
#include <stdio.h>

#include "resourceManager.h"
#include "logger.h"
#include "Graphics/shader.h"
#include "Graphics/texture.h"
#include "application.h"

namespace Lynx {

ResourceManager::~ResourceManager() 
{
	clear();
}


void ResourceManager::clear() 
{
	log_debug("Cleaning resources");

	

	for(auto const& rm : mesh_map){
		delete rm.second;
	}

	for(auto const& rm : shader_map){
		delete rm.second;
	}
}


const char* ResourceManager::getFileName(const char* path)
{
	std::string str(path);

	size_t s = str.rfind('/', str.length());
	if(s != str.npos) {
		return (str.substr(s+1, str.length() - s)).c_str();
	}

	return "";
}


Graphics::Shader* ResourceManager::LoadShader(const char* path)
{
	log_debug("e %d", shader_map.size());
	std::string name = std::string(path);
	const auto shader_found = shader_map.find(name);
	if(shader_found != shader_map.end())
		return shader_found->second;
	
	return shader_map[name] = new Graphics::Shader(path);
}


Graphics::Shader* ResourceManager::LoadShader(const char* vertex_path, const char* fragment_path)
{
	std::string name = std::string(vertex_path);
	name.append(std::string(fragment_path));
	const size_t name_hash = std::hash<std::string>{}(name);


}


void ResourceManager::th_loadTex(void* data)
{
	char* path = (char*)data;

	Graphics::Texture texture;
	texture.LoadFromFile(path);
	return;
}


Graphics::Texture ResourceManager::LoadTexture(const char* path)
{
	auto texture = FindTexture(path);
	if( texture.IsValid() )
		return texture;

	std::string name = std::string(path);

	Graphics::Texture n_texture;

	th_texdata data = {path, n_texture};
	application->m_threadPool->PushJob(th_loadTex,&data);
	
	return texture_map[name] = n_texture;
}


Graphics::Texture ResourceManager::FindTexture(const char* path)
{
	std::string name = std::string(path);
	const size_t name_hash = std::hash<std::string>{}(name);
	const auto found = texture_map.find(name);
	if(found != texture_map.end()) {
		if(found->second.IsValid())
			return found->second;
	}
	return Graphics::Texture();
}


Graphics::Mesh* ResourceManager::LoadMesh(const char* name, vector<Graphics::Vertex>* vertices, vector<unsigned int>* indices, Graphics::MeshType type)
{
	auto mesh = FindMesh(name);
	if( mesh != nullptr)
		return mesh;

	mesh_map[name] = new Graphics::Mesh(vertices, indices, type);
	return mesh_map[name];
}


Graphics::Mesh* ResourceManager::FindMesh(const char* name)
{
	const size_t name_hash = std::hash<const char*>{}(name);
	const auto found = mesh_map.find(name);
	if(found != mesh_map.end()) {
		return found->second;
	}
	return nullptr;
}

}