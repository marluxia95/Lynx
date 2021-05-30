#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <iostream>
#include <stdio.h>
#include <map>
#include "shader.h"
#include "texture.h"
#include "logger.h"

namespace Lynx {

class ResourceManager{
public:
	ResourceManager(Logger* logger);
	Texture* LoadTexture(const char* name, const char* path);
	Shader*  LoadShader(const char* name, const char* vertexPath, const char* fragmentPath);
	Texture* GetTexture(const char* name);
	Shader*  GetShader(const char* name);

	std::map<const char*, Shader*> ShaderMap;
	std::map<const char*, Texture*> TextureMap;
private:
	int textureCount;
	Logger* logger;
};

}


#endif