#ifndef SCENE_H
#define SCENE_H

#include <iostream>
#include <stdio.h>
#include <map>
#include "sprite.h"
#include "camera.h"
#include "resourceManager.h"

namespace Lynx {

class Scene{
public:
	Scene(const char* name, ResourceManager* reManager);
	void AddSprite(const char* name, Sprite* sprite);
	void AddCamera(const char* name, Camera* camera);
	void AddSprite(const char* name, const char* spriteName);
	void SetActiveCamera(const char* name);
	void Render();
	// TODO :
	// static void AddLight(const char* name, Light &light);
	// static void Add3DObject(const char* name, 3DObject &obj);

	std::map<const char*, Sprite*> Sprites;
	std::map<const char*, Camera*> Cameras;

	const char* name;
private:
	
	ResourceManager* resourceManager;
	const char* activeCamera;
};

}


#endif