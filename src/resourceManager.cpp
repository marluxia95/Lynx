#include <iostream>
#include <stdio.h>
#include <map>
#include "resourceManager.h"
#include "texture.h"
#include "shader.h"

namespace Lynx {


ResourceManager::ResourceManager(Logger* logger) : logger(logger) {

	logger->log(LOG_INFO, "Resource manager started");

}

Texture* ResourceManager::LoadTexture(const char* name, const char* path){
	TextureMap[name] = new Texture(path, textureCount);
	textureCount++;
	return TextureMap[name];
}

Shader* ResourceManager::LoadShader(const char* name, const char* vertexPath, const char* fragmentPath){
	ShaderMap[name] = new Shader(vertexPath, fragmentPath);
	if(ShaderMap[name]->success!=true){
		logger->log(LOG_ERROR, "There was an error while compiling shader");
	}
	return ShaderMap[name];
}

Texture* ResourceManager::GetTexture(const char* name){
	return TextureMap[name];
}

Shader* ResourceManager::GetShader(const char* name){
	return ShaderMap[name];
}

}