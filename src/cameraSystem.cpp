#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "simpleGameEngine.h"
#include "cameraSystem.h"
#include "entity.h"
#include "components.h"

using namespace glm;

extern Lynx::Core::Game game;

namespace Lynx {

void CameraSystem::Init()
{
	for ( auto const& entity : entities )
	{
		auto camera_component = game.GetComponent<Camera>(entity);
			
		camera_component->projection = GetProjection(camera_component);
	}
}

void CameraSystem::Update()
{
	for ( auto const& entity : entities )
	{
		auto camera_component = game.GetComponent<Camera>(entity);

		camera_component->view = GetView(entity, camera_component);
	}
}

mat4 CameraSystem::GetView(Entity entity, Camera* camera) 
{
	auto transform = game.GetComponent<Transform>(entity);
	mat4 view = mat4(1.0f);
	view = lookAt(transform->position, transform->position + camera->front, camera->up);
	return view;
}

mat4 CameraSystem::GetProjection(Camera* camera)
{
	mat4 projection;
	switch(camera->type){
		case CAMERA_ORTHOGRAPHIC:
			projection = ortho(0.0f, (float)camera->res.x / (float)camera->res.y, 0.0f, 1.0f, -1.0f, 1.0f);  
			break;
		
		case CAMERA_PERSPECTIVE:
			projection = perspective(radians(camera->FOV), (float)camera->res.x / (float)camera->res.y, 0.1f, 100.0f); 
			break;
	}
	
	

	return projection;
}

}