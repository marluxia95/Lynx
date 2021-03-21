#include <stdio.h>
#include <map>
#include <iostream>
#include "scene.h"
#include "sprite.h"
#include "camera.h"
#include "resourceManager.h"
#include "texture.h"

namespace Lynx {

Scene::Scene(const char* Name, ResourceManager* reManager){
	name = Name;
	resourceManager = reManager;
}

void Scene::AddSprite(const char* name, Sprite* sprite){
	Sprites.insert(std::make_pair(name, sprite));
}

void Scene::AddCamera(const char* name, Camera* camera){

	Cameras.insert(std::make_pair(name, camera));
	activeCamera = name;

}

void Scene::SetActiveCamera(const char* name){
	activeCamera = name;
}

void Scene::Render(){
	for(const auto &spr : Sprites){
		if(!activeCamera){
			return;
		}

		Sprite* curSprite = spr.second;
		curSprite->Draw(Cameras[activeCamera]->GetProjection());
	}

}

}

