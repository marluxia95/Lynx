#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <iostream>
#include <stdio.h>
#include <map>
#include "shader.h"
#include "texture.h"

namespace Lynx {

class ResourceManager{
public:

	static Texture* CreateTexture(const char* name, const char* path);
	static Shader*  CreateShader(const char* name, const char* vertexPath, const char* fragmentPath);
	static Texture* GetTexture(const char* name);
	static Shader*  GetShader(const char* name);

	static std::map<const char*, Shader*> ShaderMap;
	static std::map<const char*, Texture*> TextureMap;
private:
	static int textureCount;
};

}


#endif