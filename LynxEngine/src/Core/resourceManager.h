#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <iostream>
#include <future>
#include <stdio.h>
#include <memory>
#include <map>
#include <unordered_map>
#include <queue>
#include <mutex>
#include "logger.h"
#include "Graphics/mesh.h"
#include "Graphics/buffers.h"
#include "Graphics/shader.h"
#include "Graphics/texture.h"
#include "Graphics/cubemap.h"

namespace Lynx {

	class Application;

	class BaseResource : public std::enable_shared_from_this<BaseResource> {
		public:
			BaseResource(int id, const char* Path, const char* name) : id(id), path(path), name(name) {};
			int GetID() { return id; }
			const char* GetPath() { return path; }
			const char* GetName() { return name; }
			shared_ptr<BaseResource> GetPtr() { return shared_from_this(); }
		private:
			int id;
			const char* path;
			const char* name;
	};

	enum ResourceType {
		RES_SHADER,
		RES_TEXTURE,
		RES_MESH,
		RES_MODEL
	};

	class ResourceManager{
		private:
			typedef struct {
				const char* path;
				Graphics::Texture tex;
			} th_texdata;
		public:
			ResourceManager(Application* application) : application(application)
			{
				shader_map = std::unordered_map<size_t, Graphics::Shader*>();
				log_debug("e %d", shader_map.size());
			}
			~ResourceManager();
			void clear();

			void Update(float dt);

			Graphics::Shader* LoadShader(const char* path);
			Graphics::Shader* LoadShader(const char* vertex_path, const char* fragment_path);

			Graphics::Texture LoadTexture(const char* path);
			Graphics::Texture FindTexture(const char* path);

			Graphics::Mesh* LoadMesh(const char* name, vector<Graphics::Vertex>* vertices, vector<unsigned int>* indices, Graphics::MeshType type);
			Graphics::Mesh* FindMesh(const char* name);

		private:
			int lastId;
			
			std::unordered_map<size_t, Graphics::Shader*> shader_map;
			std::unordered_map<size_t, Graphics::Mesh*>   mesh_map;
			std::unordered_map<size_t, Graphics::Texture> texture_map;
			std::mutex queue_mutex;

			static void th_loadTex(void* data);
			const char* getFileName(const char* path);
			Application* application; // Application reference
	
	};

}


#endif