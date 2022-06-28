#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include "lynx.h"

#include "Core/windowManager.h"
#include "Systems/renderSystem.h"
#include "Scripting/luaRuntime.h"

#include "demo.h"
#include "demo_scene.hpp"

char Demo::title[40] = "";
Input Demo::input = Input();

Demo::Demo(int argc, char** argv)
{
	// Enables the application's debug mode
	for (int i = 1; i < argc; ++i) {
		if(std::string(argv[i]) == "--debug")
			log_set_level(LOG_DEBUG);
	}

	LoadDefaultComponents();
	LoadDefaultSystems();

	log_info("Initializing window");
	m_windowManager = std::make_shared<Lynx::WindowManager>();
	m_windowManager->Init();
	Init(NULL);

	RegisterSystem<Lynx::RenderSystem>();
    {
        Lynx::Signature signature;
        signature.set(GetComponentType<Lynx::Transform>());
        signature.set(GetComponentType<Lynx::MeshRenderer>());
        SetSystemSignature<Lynx::RenderSystem>(signature);
    }

	Lynx::EventManager::AddListener(Lynx::UpdateTick, OnUpdate);
	Lynx::EventManager::AddListener(Lynx::MouseKeyPressed, input.mouse_button_input);
	Lynx::EventManager::AddListener(Lynx::JoystickConnected, input.joystick_connected);
	Lynx::EventManager::AddListener(Lynx::JoystickDisconnected, input.joystick_disconnected);

	RegisterSystem<Lynx::Lua::LuaRuntime>();
	Lynx::ModuleManager::LoadEngineModule("LynxPhysics");

	log_info("Adding scene");
	SetScene(std::make_shared<DemoScene>());
	
	SetApplicationState(Lynx::STATE_ACTIVE);
	Run();
}

Demo::~Demo()
{

}

int Demo::OnUpdate(const Lynx::Event& ev)
{
	Lynx::GameApplication* instance = Lynx::GameApplication::GetGameInstance();
	snprintf(title,40 ,"Engine demo FPS : %d Errors : %d", (int)round(1/instance->GetDeltaTime()), log_geterrorcount());
	instance->GetWindowManager()->SetTitle(title);
	input.mouse_input();
	input.movement();
	
	return 1;
}
