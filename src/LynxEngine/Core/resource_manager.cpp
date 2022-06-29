#include <stdio.h>
#include "resource_manager.h"
#include "logger.h"
#include "Graphics/shader.h"
#include "Graphics/texture.h"
#include "Graphics/model.h"
#include "Events/async.h"
#include "application.h"

namespace Lynx {

ResourceManager::ResourceManager(ThreadPool* pool) : thpool(pool)
{
#ifdef LYNX_MULTITHREAD
	EventManager::AddListener(AsyncTextureLoad, [this](const Event& ev){
		const AsyncTextureLoadEvent& event = static_cast<const AsyncTextureLoadEvent&>(ev);
		log_debug("b");
		LYNX_ASSERT(event.Tex, "Fuck");
		event.Tex->GetResourceID();
		log_debug("a");
		event.Tex->Generate();
    });
#endif
}

ResourceManager::~ResourceManager() 
{
	Clear();
}

/**
 * @brief Clear all resources ( UNUSED )
 * 
 */
void ResourceManager::Clear() 
{
	log_debug("Cleaning resources");
}

/**
 * @brief Updates the resource queue ( UNUSED )
 * 
 */
void ResourceManager::Update(float dt)
{/*
#ifdef LYNX_MULTITHREAD
	if(texdata_queue.empty()) 
		return;

	Graphics::TextureBase* tex;
	{
		std::unique_lock<std::mutex> lock(queue_mutex);
		tex = texdata_queue.front();
	}

	log_debug("Uploading texture %s to GPU", tex->GetResourcePath());

	tex->Generate();
		
	{
		std::unique_lock<std::mutex> lock(queue_mutex);
		texdata_queue.pop();
	}
	
#endif*/
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

/**
 * @brief Loads a shader resource
 * 
 * @param vpath Vertex shader path
 * @param fpath Fragment shader path
 * @return std::shared_ptr<Graphics::Shader> 
 */
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

/**
 * @brief Loads a texture
 * 
 * @param path Texture path
 * @param type Texture type
 * @return std::shared_ptr<Graphics::TextureBase> 
 */
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
	
#ifdef LYNX_MULTITHREAD
	auto n_tex = std::make_shared<Graphics::Texture>(type);
	TexObj* obj = new TexObj{n_tex, path};

	log_debug("Texture %s", obj->path.c_str());

	log_debug("Starting to load texture %s in async mode", path);
	thpool->PushJob([this](void* data){
		TexObj* tdata = (TexObj*)data;
		std::shared_ptr<Graphics::TextureBase> tex = tdata->tex;

		log_debug("Processing texture %s", tdata->path.c_str());

		tex->LoadFromFile(tdata->path);

		EventManager::SendEvent(AsyncTextureLoadEvent(tex));

		log_debug("Added texture %s to GPU queue", tex->GetResourcePath().c_str());
		free(tdata);
		LYNX_ASSERT(tex, "Fuck");
		return;
	}, obj);
#else 
	auto n_tex = std::make_shared<Graphics::Texture>(path, type);
	n_tex->Generate();
#endif
	
	log_debug("New texture resource id : %ld", n_tex->GetResourceID());
	resource_map[ResourceBase::GetLastID()] = std::static_pointer_cast<ResourceBase>(n_tex);
	
	return n_tex;
}

/**
 * @brief Loads a mesh
 * 
 * @param name 
 * @param vertices 
 * @param indices 
 * @param type 
 * @return std::shared_ptr<Graphics::Mesh> 
 */
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

/**
 * @brief Finds a resource by its path
 * 
 * @param path 
 * @return std::shared_ptr<ResourceBase> 
 */
std::shared_ptr<ResourceBase> ResourceManager::FindResourceByPath(const std::string path)
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