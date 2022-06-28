/**
 * @file application.h
 * @author marlxx (marlxx@protonmail.com)
 * @brief This contains all the main application logic, loops, etc. 
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef APPLICATION_H
#define APPLICATION_H

#include <stdio.h>
#include <memory>

#include "resourceManager.h"
#include "windowManager.h"

#include "ECS/components.h"
#include "ECS/componentManager.h"
#include "ECS/entityManager.h"
#include "ECS/systemManager.h"

#include "scene.h"
#include "windowManager.h"
#include "resourceManager.h"
#include "threadpool.h"

#include "lynx_common.h"

namespace Lynx {

	enum State{
		STATE_ACTIVE,
        STATE_PAUSED,
        STATE_CLOSED
	};

    enum {
        APPLICATION_FULLSCREEN,
        APPLICATION_BORDERLESS = 1 << 1,
        APPLICATION_VSYNC = 1 << 2
    };

    class ResourceManager;

	class LYNXENGINE_API Application {
    public:
        Application();
		~Application();

        virtual void Init(int flags);
		virtual void Run();
        
        float GetDeltaTime() { return delta_time; }
        void InitializeSubsystems();
        void UpdateSystems() { m_systemManager->UpdateSystems(); };
        virtual void InitializeSystems() { m_systemManager->InitSystems(); }

        template<typename T>
        std::shared_ptr<T> RegisterSystem(){
            return m_systemManager->RegisterSystem<T>();
        }

        std::shared_ptr<ECS::ComponentManager> GetComponentManager() {
            return m_componentManager;
        }

        template<typename T>
        void SetSystemSignature(Signature signature){
            m_systemManager->SetSignature<T>(signature);
        }

        template<typename T>
        T* GetSystem(){
            return m_systemManager->GetSystem<T>().get();
        }

        void SetApplicationState(State state) { applicationState = state; }

        std::thread::id GetThread() { return thread_id; };
        ThreadPool* GetThreadPool() { return m_threadPool.get(); }
        static Application* GetInstance() { return s_applicationInstance; }
    protected:
        void CalculateFrameTime();

        ECS::SystemManager* GetSystemManager() 
        {
            return m_systemManager.get();
        }
    public:
        friend class ResourceManager;
        friend class ECS::SystemManager;
    private:
        static Application* s_applicationInstance;
    protected:
        float delta_time, last_FrameTime = 0.0f;
		State applicationState;
        std::thread::id thread_id;
        std::shared_ptr<ECS::ComponentManager> m_componentManager; 
        std::shared_ptr<ECS::SystemManager> m_systemManager;
        std::shared_ptr<ThreadPool> m_threadPool;	
	};

    class LYNXENGINE_API GameApplication : public Application {
    public:
        GameApplication();
        ~GameApplication();

        virtual void Init(int flags)  override;
		virtual void Run() override;

        GLFWwindow* GetWindow() { return m_windowManager->window; }

        ResourceManager* GetResourceManager(){ return m_resourceManager.get(); }

        template<typename T>
        void RegisterComponent(){
            m_componentManager->RegisterComponent<T>();
        }

        void LoadDefaultComponents();

        template<typename T>
        ECS::ComponentType GetComponentType(){
            return m_componentManager->GetComponentType<T>();
        }

        void LoadDefaultSystems();
        void SetScene(std::shared_ptr<Scene> scene);
        std::shared_ptr<Scene> GetScene() { return scene; }

        std::shared_ptr<WindowManager> GetWindowManager() { return m_windowManager; }

        unsigned int GetResolutionWidth();
        unsigned int GetResolutionHeight();

        static GameApplication* GetGameInstance() { return gameApplicationInstance; }
    private:
        static GameApplication* gameApplicationInstance;
    protected:
		std::shared_ptr<WindowManager> m_windowManager;
        std::shared_ptr<ResourceManager> m_resourceManager;
        std::shared_ptr<Scene> scene;
        EventListener scene_listener;
    }; 

}


#endif