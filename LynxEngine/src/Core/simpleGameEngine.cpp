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

    Application::~Application()
    {
        EventManager::SendEvent(LastTickEvent());
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        glfwTerminate();
    }

    void Application::Init(const char* title, unsigned int width, unsigned int height, bool fullScreen)
    {
        m_entityManager = std::make_unique<ECS::EntityManager>();
        m_componentManager = std::make_unique<ECS::ComponentManager>();
        m_systemManager = std::make_unique<ECS::SystemManager>();

        log_debug("Creating window object");

        m_windowManager = WindowManager::Create();
        m_windowManager->Init();

        log_debug("Initializing GLEW");

        log_debug("Initializing rendererAPI");

        RendererAPI::Init();

        log_debug("Initializing ImGui");

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(m_windowManager->window, true);
        ImGui_ImplOpenGL3_Init("#version 330");

        log_debug("Sending event init");

        EventManager::SendEvent(InitEvent());

        log_debug("Initializing systems");
        m_systemManager->InitSystems();

        log_debug("Successfully initialized application");
    }

    // Main Loop

    void Application::Run()
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
            
            if(applicationState == STATE_ACTIVE) {
                glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // Update logic
                EventManager::SendEvent(RenderEvent());
                m_systemManager->UpdateSystems();
            }

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            
            m_windowManager->Update();
            glfwPollEvents();
        } while(( !glfwWindowShouldClose(m_windowManager->window) ) | applicationState == STATE_CLOSED);
    }

    void Application::LoadDefaultComponents()
    {
        log_debug("Loading default components");
        RegisterComponent<Transform>();
        RegisterComponent<RigidBody>();
        RegisterComponent<Generic>();
        RegisterComponent<MeshRenderer>();
        RegisterComponent<Camera>();
        RegisterComponent<Parent>();
        RegisterComponent<PointLight>();
        RegisterComponent<DirectionalLight>();
    }

    void Application::LoadDefaultSystems()
    {
        log_debug("Loading default systems");
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

    unsigned int Application::GetResolutionWidth()
    {
        return m_windowManager->window_width;
    }

    unsigned int Application::GetResolutionHeight()
    {
        return m_windowManager->window_height;
    }


}
