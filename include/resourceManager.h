#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <iostream>
#include <stdio.h>
#include <unordered_map>
#include "shader.h"
#include "texture.h"
#include "logger.h"
#include "mesh.h"

namespace Lynx::Core {

template<typename T>
class ResourceManager{
	public:
		ResourceManager();
		T* Get(const char* name);

	private:
		std::map<const char*, Texture*> m_textures;
		std::map<const char*, Mesh*>    m_meshes;
		std::map<const char*, Shader*>  m_shaders;
};

}


#endif