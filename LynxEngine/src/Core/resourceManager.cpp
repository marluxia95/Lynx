
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
	if(!texdata_queue.empty()) {
		th_texdata* data;
		{
			std::unique_lock<std::mutex> lock(queue_mutex);
			data = texdata_queue.front();
		}

		log_debug("Uploading texture %s to GPU", data->path);
		data->tex->Generate(lastId);
		
		{
			std::unique_lock<std::mutex> lock(queue_mutex);
			texdata_queue.pop();
			free(data);
		}
	}

	if(!texdata_raw_queue.empty()) {
		th_texdata_raw data;
		{
			std::unique_lock<std::mutex> lock(queue_mutex);
			data = texdata_raw_queue.front();
		}
		if(data.baseTexture->GetTextureType() == Graphics::TEXTURE_CUBEMAP) {
			log_debug("Uploading cubemap texture %s to GPU", data.path);
			gpuUploadCubemapTexture(&data);
		}
		{
			std::unique_lock<std::mutex> lock(queue_mutex);
			texdata_raw_queue.pop();
		}
	}
}

void ResourceManager::gpuUploadCubemapTexture(th_texdata_raw* data)
{
	Graphics::CubemapTexture* cubemap_tex = (Graphics::CubemapTexture*)data->baseTexture;

	cubemap_tex->Generate(data->texdata);
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

Graphics::Texture ResourceManager::LoadTexture(const char* path)
{
	auto texture = FindTexture(path);
	if( texture.IsValid() )
		return texture;

	std::string name = std::string(path);

	Graphics::Texture* n_texture = new Graphics::Texture();
	th_texdata* data = new th_texdata({path, n_texture});

	thpool->PushJob([this](void* data){
		th_texdata* tdata = (th_texdata*)data;

		log_debug("Processing texture %s", tdata->path);

		tdata->tex->Load(tdata->path);
		{
			std::unique_lock<std::mutex> lock(queue_mutex);
			texdata_queue.push(tdata);
		}

		log_debug("Added texture %s to GPU queue", tdata->path);
		return;
	}, data);
	
	return texture_map[name] = *n_texture;
}

Graphics::TextureData* ResourceManager::AsyncLoadTextureData(const char* path, Graphics::TextureBase* baseTexture)
{
	std::string name = std::string(path);

	Graphics::TextureData* n_texture_data;

	th_texdata_raw* data = new th_texdata_raw({path, n_texture_data, baseTexture});
	thpool->PushJob([this](void* data){
		th_texdata_raw* tdata = (th_texdata_raw*)data;

		tdata->texdata = Graphics::loadTexture(tdata->path);;
		{
			std::unique_lock<std::mutex> lock(queue_mutex);
			texdata_raw_queue.push(*tdata);
			free(tdata);
		}
	}, data);
	return n_texture_data;
}

Graphics::CubemapTexture* ResourceManager::LoadCubemapTexture(std::vector<const char*>* textures)
{
	Graphics::CubemapTexture* ctex = new Graphics::CubemapTexture();
	for(int x = 0; x < textures->size(); x++) {
		auto path = textures->at(x);

		Graphics::TextureData* n_texture_data = new Graphics::TextureData();

		th_texdata_raw* data = new th_texdata_raw({path, n_texture_data, ctex});

		thpool->PushJob([this](void* data){
			th_texdata_raw* tdata = (th_texdata_raw*)data;

			log_debug("Processing cubemap texture %s", tdata->path);
			tdata->texdata = Graphics::loadTexture(tdata->path);
			{
				std::unique_lock<std::mutex> lock(queue_mutex);
				texdata_raw_queue.push(*tdata);
			}
			log_debug("Added cubemap texture %s to GPU queue", tdata->path);
		}, data);
	}
	return ctex;
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