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
#include "model.h"
#include "resourceManager.h"
#include "logger.h"
#include "light.h"

namespace Lynx {

class Scene{
public:
	Scene(const char* name, ResourceManager* resourceManager);
	Scene(const char* name, ResourceManager* resourceManager, Logger* logger);
	unsigned int AddSprite(const char* name, Sprite* sprite);
	unsigned int AddMesh(const char* name, MeshRenderer* obj);
	unsigned int Add3DObject(const char* name, Object3D* obj);
	unsigned int Add3DModel(const char* name, Model* model);
	unsigned int AddCamera(const char* name, Camera* camera);
	unsigned int AddSprite(const char* name, const char* spriteName);
	void AddPointLight(PointLight* light);
	void SetDirectionalLight(DirectionalLight* dirLight);
	void SetAmbientLight(glm::vec3 color);
	void SetActiveCamera(unsigned int id);
	void Render();
	// TODO :
	// static void AddLight(const char* name, Light &light);

	std::vector<MeshRenderer*> Meshes;
	std::vector<Object3D*> Objects;
	std::vector<Model*> Models;
	std::vector<Sprite*> Sprites;
	std::vector<Camera*> Cameras;
	std::vector<PointLight*> pointLights;
	DirectionalLight* directionalLight;
	glm::vec3 ambientLight;
	const char* name;
private:
	Logger* logger;
	ResourceManager* resourceManager;
	unsigned int activeCamera;
};

}


#endif