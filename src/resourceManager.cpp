#include <iostream>
#include <stdio.h>
#include <map>
#include "resourceManager.h"
#include "texture.h"
#include "shader.h"

namespace Lynx {

std::map<const char*, Shader* > ResourceManager::ShaderMap;
std::map<const char*, Texture*> ResourceManager::TextureMap;
int ResourceManager::textureCount;

Texture* ResourceManager::CreateTexture(const char* name, const char* path){
	Texture tex(path, textureCount);
	TextureMap[name] = &tex;
	textureCount++;
	return TextureMap[name];
}

Shader* ResourceManager::CreateShader(const char* name, const char* vertexPath, const char* fragmentPath){
	Shader shader(vertexPath, fragmentPath);
	ShaderMap[name] = &shader;
	return ShaderMap[name];
}

Texture* ResourceManager::GetTexture(const char* name){
	return TextureMap[name];
}

Shader* ResourceManager::GetShader(const char* name){
	return ShaderMap[name];
}

}