#include <stdio.h>
#include <map>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include "scene.h"
#include "sprite.h"
#include "camera.h"
#include "resourceManager.h"
#include "texture.h"
#include "mesh.h"


namespace Lynx {

Scene::Scene(const char* Name, ResourceManager* reManager){
	name = Name;
	resourceManager = reManager;
}

void Scene::AddSprite(const char* name, Sprite* sprite){
	Sprites.insert(std::make_pair(name, sprite));
}

void Scene::Add3DObject(const char* name, Mesh3D* obj) {
	Objects3D.insert(std::make_pair(name, obj));
}

void Scene::AddCamera(const char* name, Camera* camera){

	Cameras.insert(std::make_pair(name, camera));
	activeCamera = name;

}

void Scene::SetActiveCamera(const char* name){
	activeCamera = name;
}

void Scene::Render(){
	if(Cameras[activeCamera] == nullptr){printf("No active camera\n"); return;}
	if(!Sprites.empty()){
		for(const auto &spr : Sprites){
			if(!activeCamera){
				return;
			}

			Sprite* curSprite = spr.second;
			//curSprite->Draw(Cameras[activeCamera]->GetProjection(),Cameras[activeCamera]->GetView());
		}
	}
	if(!Objects3D.empty()){
		for(const auto &obj : Objects3D){
			if(!activeCamera){
				return;
			}
			obj.second->Render(Cameras[activeCamera]->GetProjection(), Cameras[activeCamera]->GetView());
		}
	}
	

}

}

