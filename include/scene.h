#ifndef SCENE_H
#define SCENE_H

#include <iostream>
#include <stdio.h>
#include <map>
#include <glm/glm.hpp>
#include "sprite.h"
#include "mesh.h"
#include "light.h"
#include "object.h"
#include "camera.h"
#include "resourceManager.h"
#include "logger.h"
#include "light.h"

namespace Lynx {

class Scene{
public:
	Scene(const char* name, ResourceManager* resourceManager);
	Scene(const char* name, ResourceManager* resourceManager, Logger* logger);
	void AddSprite(const char* name, Sprite* sprite);
	void AddMesh(const char* name, Mesh3D* obj);
	void Add3DObject(const char* name, Object3D* obj);
	void AddCamera(const char* name, Camera* camera);
	void AddSprite(const char* name, const char* spriteName);
	void AddPointLight(PointLight* light);
	void SetDirectionalLight(DirectionalLight* dirLight);
	void SetAmbientLight(glm::vec3 color);
	void SetActiveCamera(const char* name);
	void Render();
	// TODO :
	// static void AddLight(const char* name, Light &light);

	std::map<const char*, Mesh3D*> Meshes;
	std::map<const char*, Object3D*> Objects;
	std::map<const char*, Sprite*> Sprites;
	std::map<const char*, Camera*> Cameras;
	std::vector<PointLight*> pointLights;
	DirectionalLight* directionalLight;
	glm::vec3 ambientLight;
	const char* name;
private:
	Logger* logger;
	ResourceManager* resourceManager;
	const char* activeCamera;
};

}


#endif