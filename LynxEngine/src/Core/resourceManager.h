#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <iostream>
#include <stdio.h>
#include <memory>
#include <map>
#include "Graphics/mesh.h"
#include "Graphics/buffers.h"

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
		public:
			ResourceManager(Application* application) : application(application) {}
			~ResourceManager();
			void clear();

			template<class T>
			std::shared_ptr<T> LoadResource(const char* path);

			Graphics::Mesh* LoadMesh(const char* name, vector<Graphics::Vertex>* vertices, vector<unsigned int>* indices, Graphics::MeshType type);
			Graphics::Mesh* GetMesh(const char* name);

			std::map<const char*, std::shared_ptr<BaseResource>> ResourceMap;
			std::map<const char*, Graphics::Mesh*> Meshes;

		private:
			int lastId;
			const char* getFileName(const char* path);
			Application* application; // Application reference
	
	};

}


#endif