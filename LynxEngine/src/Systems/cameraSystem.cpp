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
		camera_component->CalculateProjection(resolution.x, resolution.y);
	}
}

void CameraSystem::Update()
{
	for ( auto const& entity : entities )
	{
		auto camera_component = gApplication.GetComponent<Camera>(entity);
		auto transform_component = gApplication.GetComponent<Transform>(entity);

		camera_component->CalculateView(transform_component);
	}
}

}