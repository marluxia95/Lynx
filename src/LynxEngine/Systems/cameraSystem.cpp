#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "cameraSystem.h"

#include "Core/application.h"

#include "Core/ECS/entity.h"
#include "Core/ECS/components.h"

using namespace glm;

namespace Lynx {

void CameraSystem::Init()
{
	CalculateProjections();
}

void CameraSystem::CalculateProjections()
{
	for ( auto const& entity : entities )
	{
		auto camera_component = scene->GetComponent<Camera>(entity);
			
		camera_component->Projection = GetProjection(camera_component);
	}
}

void CameraSystem::Update()
{
	for ( auto const& entity : entities )
	{
		auto camera_component = scene->GetComponent<Camera>(entity);

		camera_component->View = GetView(entity, camera_component);
	}
}

mat4 CameraSystem::GetView(EntityID entity, Camera* camera) 
{
	auto transform = scene->GetComponent<Transform>(entity);
	mat4 view = mat4(1.0f);
	view = lookAt(transform->position, transform->position + transform->rotation, camera->Up);
	return view;
}

mat4 CameraSystem::GetProjection(Camera* camera)
{
	mat4 projection;
	switch(camera->Type){
		case CAMERA_ORTHOGRAPHIC:
			projection = ortho(0.0f, (float)camera->Resolution.x / (float)camera->Resolution.y, 0.0f, 1.0f, -1.0f, 1.0f);  
			break;
		
		case CAMERA_PERSPECTIVE:
			projection = perspective(radians(camera->FOV), (float)camera->Resolution.x / (float)camera->Resolution.y, 0.1f, 1000.0f); 
			break;
	}

	return projection;
}

}