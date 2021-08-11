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
	glm::vec2 resolution = glm::vec2(gApplication.GetResolutionWidth(), gApplication.GetResolutionHeight());
	for ( auto const& entity : entities )
	{
		auto camera_component = gApplication.GetComponent<Camera>(entity);
		auto type = camera_component->type;
		switch(type) {
            case CAMERA_ORTHOGRAPHIC:
                camera_component->projection = ortho(0.0f, (float)camera_component->width / (float)camera_component->height, 0.0f, 1.0f, camera_component->near, camera_component->far);
                break;

            case CAMERA_PERSPECTIVE:
                camera_component->projection = perspective(radians(camera_component->FOV), (float)camera_component->width / (float)camera_component->height, camera_component->near, camera_component->far); 
                break;
        }
        
	}
}

void CameraSystem::Update()
{
	for ( auto const& entity : entities )
	{
		auto camera_component = gApplication.GetComponent<Camera>(entity);
		auto transform = gApplication.GetComponent<Transform>(entity);

		if(camera_component->main){
			camera_component->view = lookAt(transform->position, transform->position + transform->rotation, camera_component->up);
		}
	}
}

}