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
#include "model.h"
#include "logger.h"


namespace Lynx {

Scene::Scene(const char* name, ResourceManager* resourceManager) : name(name), resourceManager(resourceManager){

}

Scene::Scene(const char* name, ResourceManager* resourceManager, Logger* logger) : name(name), resourceManager(resourceManager), logger(logger) {

}

void Scene::AddSprite(const char* name, Sprite* sprite){
	Sprites.insert(std::make_pair(name, sprite));
}

void Scene::Add3DObject(const char* name, Object3D* obj) {
	Objects.insert(std::make_pair(name, obj));
}

void Scene::AddMesh(const char* name, Mesh3D* mesh) {
	Meshes.insert(std::make_pair(name, mesh));
}

void Scene::AddCamera(const char* name, Camera* camera){

	Cameras.insert(std::make_pair(name, camera));
	activeCamera = name;

}

void Scene::AddPointLight(PointLight* light){
	pointLights.push_back(light);
}

void Scene::SetDirectionalLight(DirectionalLight* dirLight){
	directionalLight = dirLight;
}

void Scene::SetActiveCamera(const char* name){
	activeCamera = name;
}

void Scene::Render(){
	if(Cameras[activeCamera] == nullptr){printf("No active camera\n"); return;}
	if(!activeCamera){ // Do not render if there is no camera
		return;
	}
	if(!Sprites.empty()){
		for(const auto &spr : Sprites){
			

			Sprite* curSprite = spr.second;
			//curSprite->Draw(Cameras[activeCamera]->GetProjection(),Cameras[activeCamera]->GetView());
		}
	}
	if(!Meshes.empty()){
		for(const auto &mesh : Meshes){
			
			mesh.second->Render(Cameras[activeCamera]->GetProjection(), Cameras[activeCamera]->GetView());

			if(logger && mesh.second->checkErrors() != true){
				logger->log(LOG_ERROR, "Error while rendering mesh");
			}
		}
	}
	if(!Objects.empty()){
		for(const auto &obj : Objects){
			obj.second->Render(Cameras[activeCamera]->GetProjection(), Cameras[activeCamera]->GetView(),Cameras[activeCamera]->pos,&pointLights);
		}
	}
	

}

}

