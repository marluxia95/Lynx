#include <iostream>
#include <stdio.h>
#include <future>
#include "resourceManager.h"
#include "logger.h"
#include "Graphics/shader.h"
#include "Graphics/texture.h"
#include "application.h"

namespace Lynx {

typedef struct {
	const char* name;
	const char* path;
	int id;
	ResourceType res_type;
	std::promise<BaseResource*>* res_ptr;
} ResourceData;

// Function prototypes
void th_loadRes(void* data);

ResourceManager::~ResourceManager() 
{
	clear();
}

void ResourceManager::clear() 
{
	log_debug("Cleaning resources");

	//for(auto const& res : ResourceMap){
	//	delete res.second;
	//}

	for(auto const& rm : Meshes){
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


template<class T>
std::shared_ptr<T> ResourceManager::LoadResource(const char* path)
{
	const char* name = getFileName(path);
	if(ResourceMap.find(name) != ResourceMap.end())
		return std::static_pointer_cast<T>(ResourceMap[name]);;

	std::promise<BaseResource*> res_promise;
	std::future<BaseResource*> res_future = res_promise.get_future();

	ResourceData resdata = {name, path, lastId++, RES_SHADER, &res_promise};

	application->m_threadPool->PushJob(th_loadRes, &resdata);

	ResourceMap[name] = res_future.get()->GetPtr();
	return std::static_pointer_cast<T>(ResourceMap[name]);
}


void th_loadRes(void* data)
{
	ResourceData* res_data = (ResourceData*)data;
	BaseResource* res;

	switch(res_data->res_type) {
		case RES_SHADER:
			res = new Graphics::Shader(res_data->id, res_data->path, res_data->name);
			res_data->res_ptr->set_value(res);
			return;
		case RES_TEXTURE:
			res = new Graphics::Texture(res_data->id, res_data->path, res_data->name);
			res_data->res_ptr->set_value(res);
			return;
		case RES_MODEL:
			return;
	}
	
}

Graphics::Mesh* ResourceManager::LoadMesh(const char* name, vector<Graphics::Vertex>* vertices, vector<unsigned int>* indices, Graphics::MeshType type)
{
	if(Meshes.find(name) != Meshes.end())
		return Meshes[name];

	Meshes[name] = new Graphics::Mesh(vertices, indices, type);
	return Meshes[name];
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