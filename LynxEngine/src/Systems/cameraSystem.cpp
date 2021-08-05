#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "cameraSystem.h"

#include "Core/simpleGameEngine.h"

#include "Core/ECS/entity.h"
#include "Core/ECS/components.h"

using namespace glm;

extern Lynx::Game game;

namespace Lynx {

void CameraSystem::Init()
{
	for ( auto const& entity : entities )
	{
		auto camera_component = game.GetComponent<Camera>(entity);
		auto resolution = game.GetResolution();
		camera_component->CalculateProjection(resolution.x, resolution.y);
	}
}

void CameraSystem::Update()
{
	for ( auto const& entity : entities )
	{
		auto camera_component = game.GetComponent<Camera>(entity);
		auto transform_component = game.GetComponent<Transform>(entity);

		camera_component->CalculateView(transform_component);
	}
}

}