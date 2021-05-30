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

/* NOT IMPLEMENTED YET
namespace Lynx {

Scene::Scene(const char* name, ResourceManager* resourceManager) : name(name), resourceManager(resourceManager){

}

Scene::Scene(const char* name, ResourceManager* resourceManager, Logger* logger) : name(name), resourceManager(resourceManager), logger(logger) {

}

unsigned int Scene::AddSprite(const char* name, Sprite* sprite){
	unsigned int id = Sprites.size();
	Sprites.push_back(sprite);
	Sprites[id]->name = name;
	return id;
}

unsigned int Scene::Add3DObject(const char* name, Object3D* obj) {
	unsigned int id = Objects.size();
	Objects.push_back(obj);
	Objects[id]->name = name;
	return id;
}

unsigned int Scene::Add3DModel(const char* name, Model* model) {
	unsigned int id = Models.size();
	Models.push_back(model);
	Models[id]->name = name;
	return id;
}

unsigned int Scene::AddMesh(const char* name, MeshRenderer* mesh) {
	unsigned int id = Meshes.size();
	Meshes.push_back(mesh);
	Meshes[id]->name = name;
	return id;
}

unsigned int Scene::AddCamera(const char* name, Camera* camera){
	unsigned int id = Cameras.size();
	Cameras.push_back(camera);
	activeCamera = id;
	Cameras[id]->name = name;
	return id;
}

void Scene::AddPointLight(PointLight* light){
	pointLights.push_back(light);
}

void Scene::SetDirectionalLight(DirectionalLight* dirLight){
	directionalLight = dirLight;
}

void Scene::SetActiveCamera(unsigned int id){
	activeCamera = id;
}

void Scene::Render(){
	if(Cameras[activeCamera] == nullptr){logger->log(LOG_WARN, "No active camera"); return;}
	if(!Sprites.empty()){
		for(const auto &spr : Sprites){
			spr->Render(Cameras[activeCamera]->GetProjection(),Cameras[activeCamera]->GetView());
		}
	}
	if(!Meshes.empty()){
		for(const auto &mesh : Meshes){
			
			mesh->Render(Cameras[activeCamera]->GetProjection(), Cameras[activeCamera]->GetView());

			if(logger && mesh->checkErrors() != true){
				logger->log(LOG_ERROR, "Error while rendering mesh");
			}
		}
	}
	if(!Models.empty()){
		for(const auto &mdl : Models){
			mdl->Render(Cameras[activeCamera]->GetProjection(), Cameras[activeCamera]->GetView());
		}
	}
	if(!Objects.empty()){
		for(const auto &obj : Objects){
			obj->Render(Cameras[activeCamera]->GetView(), Cameras[activeCamera]->GetProjection(),Cameras[activeCamera]->pos,&pointLights);
		}
	}
	

}

}
*/
