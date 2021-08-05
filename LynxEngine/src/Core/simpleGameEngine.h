#ifndef SIMPLEGAMEENGINE_H
#define SIMPLEGAMEENGINE_H

#include <stdio.h>
#include <iostream>
#include <vector>
#include <map>
#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <memory>

#include "resourceManager.h"
#include "windowManager.h"

#include "Graphics/renderer.h"
#include "Graphics/model.h"
#include "Graphics/mesh.h"

#include "ECS/systemManager.h"
#include "ECS/componentManager.h"
#include "ECS/entityManager.h"
#include "ECS/systemManager.h"

#include "Events/event.h"

#include "windowManager.h"

#include "Systems/parentingSystem.h"
#include "Systems/lightingSystem.h"
#include "Systems/physicsSystem.h"
#include "Systems/renderSystem.h"
#include "Systems/cameraSystem.h"

namespace Lynx {

	enum GameState{
		GAME_PAUSE,
		GAME_ACTIVE
	};
	
	class Game {
	public:
		~Game();

		void Init();
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

        glm::vec2 GetResolution();

        void LoadDefaultComponents();
        void LoadDefaultSystems();


		float delta_time, last_FrameTime;
	private:
		bool running;
		std::unique_ptr<ECS::EntityManager> m_entityManager;
        std::unique_ptr<ECS::ComponentManager> m_componentManager;
		std::unique_ptr<WindowManager> m_windowManager;
		std::shared_ptr<ECS::SystemManager> m_systemManager;

	};

}
#endif
