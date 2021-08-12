#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "cameraSystem.h"

#include "Core/simpleGameEngine.h"

#include "Core/ECS/entity.h"
#include "Core/ECS/components.h"

using namespace glm;

extern Lynx::Application gApplication;

namespace Lynx {

void CameraSystem::Init()
{
	CalculateProjections();
}

void CameraSystem::CalculateProjections()
{
	for ( auto const& entity : entities )
	{
		auto camera_component = gApplication.GetComponent<Camera>(entity);
			
		camera_component->projection = GetProjection(camera_component);
	}
}

void CameraSystem::Update()
{
	for ( auto const& entity : entities )
	{
		auto camera_component = gApplication.GetComponent<Camera>(entity);

		camera_component->view = GetView(entity, camera_component);
	}
}

mat4 CameraSystem::GetView(Entity entity, Camera* camera) 
{
	auto transform = gApplication.GetComponent<Transform>(entity);
	mat4 view = mat4(1.0f);
	view = lookAt(transform->position, transform->position + transform->rotation, camera->up);
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
			projection = perspective(radians(camera->FOV), (float)camera->res.x / (float)camera->res.y, 0.1f, 1000.0f); 
			break;
	}

	return projection;
}

}