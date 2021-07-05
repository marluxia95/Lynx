#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <iostream>
#include <vector>
#include <glm/glm.hpp>

#include "Core/simpleGameEngine.h"
#include "Core/windowManager.h"
#include "Core/eventManager.h"

#include "Graphics/texture.h"
#include "Graphics/model.h"

#include "ECS/components.h"

using namespace Lynx;

// Initialize global variables

WindowManager gWindowManager;
EventManager gEventManager;
ResourceManager gResourceManager;
Game game;

void Init()
{

}

void Update()
{

}

int main()
{

	// Initialize window in windowed mode
	gWindowManager.Init("Example Game", 1920, 1080, true);

	// Enables the game's debug mode
	game.SetDebugMode(true);
	game.Init();
	log_set_level(LOG_DEBUG);

	// Runs the game
	game.Run();
	gWindowManager.Destroy();
	return 0;
}