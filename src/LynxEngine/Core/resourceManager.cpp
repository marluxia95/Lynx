#include <stdio.h>
#include "resourceManager.h"
#include "logger.h"
#include "Graphics/shader.h"
#include "Graphics/texture.h"
#include "application.h"

namespace Lynx {

ResourceManager::ResourceManager(ThreadPool* pool) : thpool(pool)
{
	
}

ResourceManager::~ResourceManager() 
{
	Clear();
}


void ResourceManager::Clear() 
{
	log_debug("Cleaning resources");

	for(auto const& rm : mesh_map){
		delete rm.second;
	}

	for(auto const& rm : shader_map){
		delete rm.second;
	}
}

void ResourceManager::Update(float dt)
{
#ifdef LYNX_MULTITHREAD
	if(!texdata_queue.empty()) {
		Graphics::TextureBase* tex;
		{
			std::unique_lock<std::mutex> lock(queue_mutex);
			tex = texdata_queue.front();
		}

		log_debug("Uploading texture %s to GPU", tex->GetPath());

		tex->Generate();
		
		{
			std::unique_lock<std::mutex> lock(queue_mutex);
			texdata_queue.pop();
		}
	}
#endif
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
	//WIP
	return NULL;
}

Graphics::Texture ResourceManager::LoadTexture(const char* path)
{
	auto texture = FindTexture(path);
	if( texture.IsValid() )
		return texture;

	std::string name = std::string(path);

	Graphics::Texture n_texture = Graphics::Texture(path);
	
#ifdef LYNX_MULTITHREAD
	log_debug("Starting to load texture %s in async mode", n_texture.GetPath());
	thpool->PushJob([this](void* data){
		Graphics::TextureData* tdata = (Graphics::TextureData*)data;

		log_debug("Processing texture %s", tdata->GetPath());

		tdata->linkedTexture->Load();

		{
			std::unique_lock<std::mutex> lock(queue_mutex);
			texdata_queue.push(tdata->linkedTexture);
		}
		log_debug("Added texture %s to GPU queue", tdata->GetPath());
		return;
	}, n_texture.GetData());
#else
	log_debug("Loading texture %s", n_texture.GetPath());
	n_texture.Load();
	n_texture.Generate();
#endif
	
	return texture_map[name] = n_texture;
}

Graphics::CubemapTexture ResourceManager::LoadCubemapTexture(std::vector<const char*>* textures)
{
	Graphics::CubemapTexture ctex = Graphics::CubemapTexture();
	for(int x = 0; x < textures->size(); x++) {
		auto path = textures->at(x);

#ifdef LYNX_MULTITHREAD
		log_debug("Adding cubemap texture %s to queue", path);

		thpool->PushJob([this](void* data){
			Graphics::TextureData* tdata = (Graphics::TextureData*)data;

			log_debug("Processing cubemap texture %s", tdata->GetPath());
			

			tdata->linkedTexture->Load();
			{
				std::unique_lock<std::mutex> lock(queue_mutex);
				texdata_queue.push(tdata->linkedTexture);
			}
			free(tdata);
			log_debug("Added cubemap texture %s to GPU queue", tdata->GetPath());
		}, ctex.GetData());
#else
		ctex.Load(path);
		ctex.Generate();
#endif
	}
	return ctex;
}

Graphics::Texture ResourceManager::FindTexture(const char* path)
{
	std::string name = std::string(path);
	const auto found = texture_map.find(name);
	if(found != texture_map.end()) {
		if(found->second.IsValid())
			return found->second;
	}
	return Graphics::Texture();
}


Graphics::Mesh* ResourceManager::LoadMesh(const char* name, std::vector<Graphics::Vertex>* vertices, std::vector<unsigned int>* indices, Graphics::MeshType type)
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