#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <stdio.h>
#include <unordered_map>
#include <queue>
#include <mutex>
#include "logger.h"
#include "threadpool.h"
#include "Graphics/mesh.h"
#include "Graphics/buffers.h"
#include "Graphics/shader.h"
#include "Graphics/texture.h"
#include "Graphics/cubemap.h"

namespace Lynx {

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
				Graphics::Texture* tex;
			} th_texdata;

			typedef struct {
				const char* path;
				Graphics::TextureData* texdata;
				Graphics::TextureBase* baseTexture;
			} th_texdata_raw;
		public:
			ResourceManager(ThreadPool* pool);
			~ResourceManager();
			void clear();

			void Update(float dt);

			Graphics::Shader* LoadShader(const char* path);
			Graphics::Shader* LoadShader(const char* vertex_path, const char* fragment_path);
			int GetShaderCount() { return shader_map.size(); }

			Graphics::Texture LoadTexture(const char* path);
			Graphics::Texture FindTexture(const char* path);

			Graphics::CubemapTexture* LoadCubemapTexture(std::vector<const char*>* textures);

			Graphics::TextureData* AsyncLoadTextureData(const char* path, Graphics::TextureBase* baseTexture);

			Graphics::Mesh* LoadMesh(const char* name, vector<Graphics::Vertex>* vertices, vector<unsigned int>* indices, Graphics::MeshType type);
			Graphics::Mesh* FindMesh(const char* name);

		private:
			int lastId;
			
			std::unordered_map<std::string, Graphics::Shader*> shader_map = {};
			std::unordered_map<std::string, Graphics::Mesh*>   mesh_map = {};
			std::unordered_map<std::string, Graphics::Texture> texture_map = {};

			std::queue<th_texdata*> texdata_queue;
			std::queue<th_texdata_raw*> texdata_raw_queue;
			std::mutex queue_mutex;

			void gpuUploadCubemapTexture(th_texdata_raw* data);
			//static void async_loadCubemapTexData(void* data);
			const char* getFileName(const char* path);

			ThreadPool* thpool;
	
	};

}


#endif