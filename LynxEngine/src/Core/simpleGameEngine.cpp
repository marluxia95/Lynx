#include <stdio.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "simpleGameEngine.h"
#include "windowManager.h"

#include "eventManager.h"
#include "Events/event.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Graphics/mesh.h"

#include "Systems/lightingSystem.h"
#include "Systems/parentingSystem.h"
#include "Systems/cameraSystem.h"
#include "Systems/renderSystem.h"

#include "ECS/systemManager.h"
#include "ECS/components.h"

#include "logger.h"

namespace Lynx {

Game::~Game()
{
    EventManager::SendEvent(LastTickEvent());
	ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
	glfwTerminate();
}

void Game::Init()
{
    m_windowManager = WindowManager::Create();
	
	bool err = glewInit() != GLEW_OK;   

    if(err){
    	log_fatal("Failed to initalize GLEW");
		exit(1);
    }

	

    RendererAPI::Init();


	IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(m_windowManager->window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    EventManager::SendEvent(InitEvent());

    m_systemManager->InitSystems();
}

// Main Loop

void Game::Run()
{
	do
	{
        float current_FrameTime = glfwGetTime();
		delta_time = current_FrameTime - last_FrameTime;
		last_FrameTime = current_FrameTime;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

		EventManager::SendEvent(UpdateTickEvent());
        
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		EventManager::SendEvent(RenderEvent());
        m_systemManager->UpdateSystems();

        ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        m_windowManager->Update();
        glfwPollEvents();
    } while((!glfwWindowShouldClose(m_windowManager->window))|running);
}

void Game::LoadDefaultComponents()
{
    RegisterComponent<Transform>();
    RegisterComponent<RigidBody>();
    RegisterComponent<Generic>();
    RegisterComponent<MeshRenderer>();
    RegisterComponent<Camera>();
	RegisterComponent<Parent>();
	RegisterComponent<PointLight>();
    RegisterComponent<DirectionalLight>();
}

void Game::LoadDefaultSystems()
{
    RegisterSystem<RenderSystem>();
    {
    	Signature signature;
    	signature.set(GetComponentType<Transform>());
    	signature.set(GetComponentType<MeshRenderer>());
    	SetSystemSignature<RenderSystem>(signature);
    }

    RegisterSystem<CameraSystem>();
    {
    	Signature signature;
    	signature.set(GetComponentType<Transform>());
    	signature.set(GetComponentType<Camera>());
    	SetSystemSignature<CameraSystem>(signature);
    }

    RegisterSystem<PhysicsSystem>();
    {
    	Signature signature;
    	signature.set(GetComponentType<Transform>());
    	signature.set(GetComponentType<RigidBody>());
    	SetSystemSignature<PhysicsSystem>(signature);
    }

    RegisterSystem<LightingSystem>();
	{
		Signature signature;
		signature.set(GetComponentType<Transform>());
		signature.set(GetComponentType<PointLight>());
		SetSystemSignature<LightingSystem>(signature);
	}
}

}
