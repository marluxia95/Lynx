#include <stdio.h>
#include "resourceManager.h"
#include "logger.h"
#include "Graphics/shader.h"
#include "Graphics/texture.h"
#include "Graphics/model.h"
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

std::shared_ptr<Graphics::Shader> ResourceManager::LoadShader(const char* vpath, const char* fpath)
{
	std::string name = std::string(vpath);
	const auto shader_found = GetResource<Graphics::Shader>(vpath);
	if(shader_found != NULL)
		return shader_found;

	auto n_shader = std::make_shared<Graphics::Shader>(vpath, fpath);
	resource_map[ResourceBase::GetLastID()] = std::static_pointer_cast<ResourceBase>(n_shader);
	
	return n_shader;
}

std::shared_ptr<Graphics::TextureBase> ResourceManager::LoadTexture(const char* path, Graphics::TextureType type)
{
	std::shared_ptr<Graphics::TextureBase> texture = GetResource<Graphics::TextureBase>(path);
	if( texture ) {
		log_debug("Found texture in cache");
		if( texture->IsValid() )
			return texture;
	}

	log_debug("Didn't find a valid texture in cache, loading");

	std::string name = std::string(path);

	log_debug("Calling constructor of texture object");
	auto n_tex = std::make_shared<Graphics::Texture>(path, type);
	log_debug("New texture resource id : %ld", n_tex->GetResourceID());
	
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
#endif
	
	resource_map[ResourceBase::GetLastID()] = std::static_pointer_cast<ResourceBase>(n_tex);
	
	return n_tex;
}

std::shared_ptr<Graphics::Mesh> 
ResourceManager::LoadMesh(const char* name, std::vector<Graphics::Vertex>* vertices, 
	std::vector<unsigned int>* indices, 
	Graphics::MeshType type)
{
	std::string res_name = name;
	const auto found = GetResource<Graphics::Mesh>(name);
	if(found != NULL)
		return found;

	auto n_shader = std::make_shared<Graphics::Mesh>(name, vertices, indices, type);
	resource_map[ResourceBase::GetLastID()] = std::static_pointer_cast<ResourceBase>(n_shader);
	
	return n_shader;
}

std::shared_ptr<ResourceBase> ResourceManager::FindResourceByPath(std::string path)
{
	for(auto const& [k,v] : resource_map) {
		log_debug("checking %ld from %s", k, v->GetResourcePath().c_str());
		if(v->GetResourcePath() == path) {
			log_debug("Found matching resource in cache : %d", v->GetResourceID());
			return v;
		}
	}
	return NULL;
}

}