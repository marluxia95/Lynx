#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <iostream>
#include <vector>
#include <glm/glm.hpp>

#include "lynx.h"
#include "Systems/renderSystem.h"
#include "Scripting/luaRuntime.h"
#include "Graphics/cubemap.h"

#include "demo_scene.hpp"
#include "input.hpp"

char title[40];

Lynx::GameApplication* applicationInstance;

int Update(const Lynx::Event& ev)
{
	//snprintf(title,40 ,"Engine demo FPS : %d Errors : %d", (int)round(1/applicationInstance->GetDeltaTime()), log_geterrorcount());
	//glfwSetWindowTitle(applicationInstance->GetWindow(), title);
	mouse_input();
	movement();
	
	return 1;
}

int main(int argc, char** argv)
{
	// Enables the application's debug mode
	for (int i = 1; i < argc; ++i) {
		if(std::string(argv[i]) == "--debug")
			log_set_level(LOG_DEBUG);
	}

	applicationInstance = new Lynx::GameApplication();

	log_info("Initializing window");

	// Initialize window in windowed mode
	
	applicationInstance->LoadDefaultComponents();
	applicationInstance->LoadDefaultSystems();

	applicationInstance->Init("Example", 1920, 1080, false);

	applicationInstance->RegisterSystem<Lynx::RenderSystem>();
    {
        Lynx::Signature signature;
        signature.set(applicationInstance->GetComponentType<Lynx::Transform>());
        signature.set(applicationInstance->GetComponentType<Lynx::MeshRenderer>());
        applicationInstance->SetSystemSignature<Lynx::RenderSystem>(signature);
    }

    applicationInstance->RegisterSystem<Lynx::Lua::LuaRuntime>();
    {
    	Lynx::Signature signature;
        signature.set(applicationInstance->GetComponentType<Lynx::LuaScript>());
        applicationInstance->SetSystemSignature<Lynx::Lua::LuaRuntime>(signature);
    }

	log_debug("Adding initial events...");
	Lynx::EventManager::AddListener(Lynx::UpdateTick, Update);
	Lynx::EventManager::AddListener(Lynx::MouseKeyPressed, mouse_button_input);
	Lynx::EventManager::AddListener(Lynx::JoystickConnected, joystick_connected);
	Lynx::EventManager::AddListener(Lynx::JoystickDisconnected, joystick_disconnected);

	log_info("Adding scene");
	applicationInstance->SetScene(std::make_shared<DemoScene>());

	// Runs the application
	applicationInstance->SetApplicationState(Lynx::STATE_ACTIVE);
	applicationInstance->Run();

	return 0;
}
