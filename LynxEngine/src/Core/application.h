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

	class Application {
    public:
        Application();
		~Application();

        virtual void Init(const char* title, unsigned int width, unsigned int height, int flags);
		virtual void Run();
        
        void InitializeSubsystems();
        void CalculateFrameTime();
        void UpdateSystems() { m_systemManager->UpdateSystems(); };
        virtual void InitializeSystems() { m_systemManager->InitSystems(); }

        template<typename T>
        std::shared_ptr<T> RegisterSystem(){
            return m_systemManager->RegisterSystem<T>();
        }

        template<typename T>
        void SetSystemSignature(Signature signature){
            m_systemManager->SetSignature<T>(signature);
        }

        template<typename T>
        T* GetSystem(){
            return m_systemManager->GetSystem<T>().get();
        }

        std::thread::id GetThread() { return thread_id; };
        static Application* GetInstance() { return s_applicationInstance; }

		float delta_time, last_FrameTime;
        friend class ResourceManager;
    private:
        static Application* s_applicationInstance;
    protected:
		State applicationState;
        std::thread::id thread_id;
        std::shared_ptr<ECS::SystemManager> m_systemManager;
        std::shared_ptr<ThreadPool> m_threadPool;	
	};

    class GameApplication : public Application {
    public:
        GameApplication();
        ~GameApplication();

        virtual void Init(const char* title, unsigned int width, unsigned int height, int flags)  override;
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

        void EntitySignatureChanged(Entity entity, Signature signature) { m_systemManager->EntitySignatureChanged(entity, signature); }
        void EntityDestroyed(Entity entity) { m_systemManager->EntityDestroyed(entity);}

        void LoadDefaultSystems();
        Scene* CreateScene();

        unsigned int GetResolutionWidth();
        unsigned int GetResolutionHeight();

        static GameApplication* GetGameInstance() { return gameApplicationInstance; }
    private:
        static GameApplication* gameApplicationInstance;
    protected:
        std::shared_ptr<ECS::ComponentManager> m_componentManager; 
		std::unique_ptr<WindowManager> m_windowManager;
        std::shared_ptr<ResourceManager> m_resourceManager;
        Scene* scene;
    }; 

}


#endif