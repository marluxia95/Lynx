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

#include "Systems/lightingSystem.h"
#include "Systems/cameraSystem.h"
#include "Systems/renderSystem.h"
#include "Systems/transformSystem.h"

#include "ECS/systemManager.h"
#include "ECS/components.h"
#include "module.h"

#include "logger.h"

#ifdef __linux__
#include <unistd.h>
#endif

namespace Lynx {

    Application* Application::s_applicationInstance = nullptr;
    GameApplication* GameApplication::gameApplicationInstance = nullptr;

    Application::Application() : m_componentManager(std::make_unique<ECS::ComponentManager>()), m_systemManager(std::make_unique<ECS::SystemManager>())
    {
        log_debug("Initializing subsystems");

        thread_id = std::this_thread::get_id();

#ifdef LYNX_MULTITHREAD
        log_warn("Multithreading is enabled ! Keep in mind that this is still in progress and the application might not work as intended !");
        m_threadPool = std::make_shared<ThreadPool>(3);
#endif

        s_applicationInstance = this;

        EventManager::AddListener(SignatureChanged, [this](const Event& ev){
            const SignatureChangedEvent& event = static_cast<const SignatureChangedEvent&>(ev);
            m_systemManager->EntitySignatureChanged(event.entity, event.signature);
        });

        EventManager::AddListener(EntityDestroyed, [this](const Event& ev){
            const EntityDestroyedEvent& event = static_cast<const EntityDestroyedEvent&>(ev);
            m_systemManager->EntityDestroyed(event.entity);
        });
    }

    Application::~Application()
    {
        ModuleManager::UnloadAllModules();
#ifdef LYNX_MULTITHREAD
        m_threadPool->Wait();
#endif
    }

    /**
     * @brief Initializes the application instance
     * 
     * @param flags Not used
     */
    void Application::Init(int flags)
    {
        log_debug("Sending event init");
        EventManager::SendEvent(InitEvent());

        log_debug("Initializing systems");
        m_systemManager->InitSystems();

        log_debug("Successfully initialized application");
    }

    /**
     * @brief Calculates the frametime
     * 
     */
    void Application::CalculateFrameTime()
    {
        float current_FrameTime = glfwGetTime();
        delta_time = current_FrameTime - last_FrameTime;
        last_FrameTime = current_FrameTime;
    }

    /**
     * @brief Runs the main loop
     * 
     */
    void Application::Run()
    {
        do
        {
            CalculateFrameTime();
#ifdef LYNX_MULTITHREAD
            EventManager::UpdateListeners();
#endif
            if(applicationState == STATE_ACTIVE) {
                EventManager::SendEvent(UpdateTickEvent());

                // Update logic
                EventManager::SendEvent(RenderEvent());
                UpdateSystems();
            }

        } while(applicationState != STATE_CLOSED);
        EventManager::SendEvent(LastTickEvent());
    }

    GameApplication::GameApplication() : Application(), m_resourceManager(std::make_shared<ResourceManager>(m_threadPool.get()))
    {
        gameApplicationInstance = this;
    }

    GameApplication::~GameApplication()
    {
        if(m_windowManager != nullptr)
            m_windowManager->Destroy();
    }

    /**
     * @brief Initializes the game application
     * 
     * @param title Window title
     * @param width Window width
     * @param height Window height
     * @param flags Window flags
     */
    void GameApplication::Init(int flags)
    {

        Graphics::RendererAPI::Init();

        Input::Init();

        log_debug("Sending event init");
        EventManager::SendEvent(InitEvent());

        log_debug("Initializing systems");
        m_systemManager->InitSystems();

        scene = std::make_shared<Scene>();

        log_debug("Successfully initialized application");
    }

    /**
     * @brief Loads the game's default systems
     * 
     */
    void GameApplication::LoadDefaultSystems()
    {
        log_debug("Loading default systems");
        RegisterSystem<TransformSystem>();
        {
            Signature signature;
            signature.set(GetComponentType<Transform>());
            SetSystemSignature<TransformSystem>(signature);
        }

        RegisterSystem<CameraSystem>();
        {
            Signature signature;
            signature.set(GetComponentType<Transform>());
            signature.set(GetComponentType<Camera>());
            SetSystemSignature<CameraSystem>(signature);
        }

        RegisterSystem<LightingSystem>();
        {
            Signature signature;
            signature.set(GetComponentType<Transform>());
            signature.set(GetComponentType<PointLight>());
            SetSystemSignature<LightingSystem>(signature);
        }
    }

    /**
     * @brief Returns the window's resulution width
     * 
     * @return unsigned int 
     */
    unsigned int GameApplication::GetResolutionWidth()
    {
        return m_windowManager->window_width;
    }

    /**
     * @brief Returns the window's resuliotn height
     * 
     * @return unsigned int 
     */
    unsigned int GameApplication::GetResolutionHeight()
    {
        return m_windowManager->window_height;
    }

    /**
     * @brief Runs the game's main loop
     * 
     */
    void GameApplication::Run()
    {
        EventManager::SendEvent(FirstTickEvent());
        do
        {
            CalculateFrameTime();

#ifdef LYNX_MULTITHREAD
            EventManager::UpdateListeners();
#endif
            
            if(applicationState == STATE_ACTIVE) {
                EventManager::SendEvent(UpdateTickEvent());

                glClearColor(0, 0, 0, 0);
		        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // Update logic
                EventManager::SendEvent(RenderEvent());
                m_resourceManager->Update(delta_time);
                UpdateSystems();
            }

            m_windowManager->Update();
        } while(( !glfwWindowShouldClose(m_windowManager->window) ) | applicationState == STATE_CLOSED);
        EventManager::SendEvent(LastTickEvent());
    }

    /**
     * @brief Sets the game's current scene
     * 
     * @param n_scene 
     */
    void GameApplication::SetScene(std::shared_ptr<Scene> n_scene)
    {
        if(scene_listener.GetID())
            EventManager::RemoveListener(LastTick, scene_listener);
        
        scene = n_scene;
        scene_listener = EventManager::AddListener(LastTick, [n_scene](const Event& ev){n_scene->Destroy();});
        m_systemManager->SetScene(n_scene);
        n_scene->Init();
    }

    /**
     * @brief Loads the default components
     * 
     */
    void GameApplication::LoadDefaultComponents()
    {
        log_debug("Loading default components");
        RegisterComponent<Transform>();
        RegisterComponent<Generic>();
        RegisterComponent<Parent>();
        RegisterComponent<MeshRenderer>();
        RegisterComponent<Camera>();
        RegisterComponent<Children>();
        RegisterComponent<PointLight>();
        RegisterComponent<DirectionalLight>();
    }

}
