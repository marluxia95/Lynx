#include <stdio.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "application.h"
#include "windowManager.h"
#include "inputManager.h"

#include "eventManager.h"
#include "Events/event.h"

#include "Graphics/rendererAPI.h"
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
        glfwTerminate();
    }

    void Application::Init(const char* title, unsigned int width, unsigned int height, int flags)
    {
        log_debug("Creating window object");

        m_windowManager = WindowManager::Create();
        if(flags & APPLICATION_FULLSCREEN)
            m_windowManager->Init(title, width, height, true);

        m_threadPool = std::make_unique<ThreadPool>();
        m_resourceManager = std::make_unique<ResourceManager>(this);
        m_entityManager = std::make_unique<ECS::EntityManager>();
        m_componentManager = std::make_unique<ECS::ComponentManager>();
        m_systemManager = std::make_unique<ECS::SystemManager>();
        
        log_debug("Initializing renderer API");

        Graphics::RendererAPI::Init();

        Input::Init();

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
            
            if(applicationState == STATE_ACTIVE) {
                EventManager::SendEvent(UpdateTickEvent());

                Graphics::RendererAPI::Clear(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f));

                // Update logic
                EventManager::SendEvent(RenderEvent());
                m_systemManager->UpdateSystems();
            }

            m_windowManager->Update();
            glfwPollEvents();
        } while(( !glfwWindowShouldClose(m_windowManager->window) ) | applicationState == STATE_CLOSED);
        EventManager::SendEvent(LastTickEvent());
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
