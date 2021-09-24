#ifndef APPLICATION_H
#define APPLICATION_H

#include <stdio.h>
#include <memory>

#include "resourceManager.h"
#include "windowManager.h"

#include "ECS/components.h"
#include "ECS/systemManager.h"
#include "ECS/componentManager.h"
#include "ECS/entityManager.h"
#include "ECS/systemManager.h"

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

        void Init(const char* title = "Lynx Engine", unsigned int width = 1270, unsigned int height = 720, int flags = 0);
		void Run();

	
        Entity CreateEntity(const char* name) {
            Entity ent = m_entityManager->CreateEntity();
            m_componentManager->AddComponent(ent, Generic{name});
			return ent;
        }

        Entity CreateEntity() {
            Entity ent = m_entityManager->CreateEntity();
            return ent;
        }

        void DestroyEntity(Entity entity) {
            m_entityManager->DestroyEntity(entity);
            m_systemManager->EntityDestroyed(entity);
            m_componentManager->EntityDestroyed(entity);
        }

        template<typename T>
        void RegisterComponent(){
            m_componentManager->RegisterComponent<T>();
        }

        template<typename T>
        void AddComponent(Entity entity, T component){
            m_componentManager->AddComponent(entity, component);

            auto signature = m_entityManager->GetSignature(entity);
            signature.set(m_componentManager->GetComponentType<T>(), true);
            m_entityManager->SetSignature(entity, signature);

            m_systemManager->EntitySignatureChanged(entity, signature);
        }

        template<typename T>
        void RemoveComponent(Entity entity){
            m_componentManager->RemoveComponent<T>(entity);

            auto signature = m_entityManager->GetSignature(entity);
            signature.set(m_componentManager->GetComponentType<T>(), false);
            m_entityManager->SetSignature(entity, signature);

            m_systemManager->EntitySignatureChanged(entity, signature);
        }

        template<typename T>
        T* GetComponent(Entity entity){
            return m_componentManager->GetComponent<T>(entity);
        }

        template<typename T>
        ECS::ComponentType GetComponentType(){
            return m_componentManager->GetComponentType<T>();
        }

        int GetEntityCount() { return m_entityManager->livingEntityCount; }
            

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

        ResourceManager* GetResourceManager()
        {
            return m_resourceManager.get();
        }

        unsigned int GetResolutionWidth();
        unsigned int GetResolutionHeight();

        void LoadDefaultComponents();
        void LoadDefaultSystems();

        GLFWwindow* GetWindow() { return m_windowManager->window; }

		float delta_time, last_FrameTime;
        friend class ResourceManager;
	private:
		State applicationState;
		std::unique_ptr<ECS::EntityManager> m_entityManager;
        std::unique_ptr<ECS::ComponentManager> m_componentManager;
        std::unique_ptr<ECS::SystemManager> m_systemManager;
		std::unique_ptr<WindowManager> m_windowManager;
        std::shared_ptr<ThreadPool> m_threadPool;
        std::shared_ptr<ResourceManager> m_resourceManager;
	};

}

#endif