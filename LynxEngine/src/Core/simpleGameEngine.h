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

#include "ECS/entity.h"

#include "ECS/componentManager.h"
#include "ECS/entityManager.h"
#include "ECS/systemManager.h"
#include "ECS/system.h"

#include "Events/event.h"

#include "windowManager.h"

#include "Editor/editor.h"

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
		void SetDebugMode(bool mode);
		
		Entity CreateEntity();
		Entity CreateEntity(const char* name);
		void DestroyEntity(Entity entity);

		template<typename T>
		void RegisterComponent(){
			componentManager->RegisterComponent<T>();
		}

		template<typename T>
		void AddComponent(Entity entity, T component){
			componentManager->AddComponent(entity, component);

			auto signature = entityManager->GetSignature(entity);
			signature.set(componentManager->GetComponentType<T>(), true);
			entityManager->SetSignature(entity, signature);

			systemManager->EntitySignatureChanged(entity, signature);
		}

		template<typename T>
		void RemoveComponent(Entity entity){
			componentManager->RemoveComponent<T>(entity);

			auto signature = entityManager->GetSignature(entity);
			signature.set(componentManager->GetComponentType<T>(), false);
			entityManager->SetSignature(entity, signature);

			systemManager->EntitySignatureChanged(entity, signature);
		}

		template<typename T>
		T* GetComponent(Entity entity){
			return componentManager->GetComponent<T>(entity);
		}

		template<typename T>
		ECS::ComponentType GetComponentType(){
			return componentManager->GetComponentType<T>();
		}

		template<typename T>
		std::shared_ptr<T> RegisterSystem(){
			return systemManager->RegisterSystem<T>();
		}

		template<typename T>
		void SetSystemSignature(Signature signature){
			systemManager->SetSignature<T>(signature);
		}

		int GetEntityCount();
		std::shared_ptr<RenderSystem> renderSystem;
		std::shared_ptr<PhysicsSystem> physicsSystem;
		std::shared_ptr<LightingSystem> lightingSystem;
		
		float delta_time, last_FrameTime;

		friend class Lynx::Editor;
	private:
		bool running;

		std::unique_ptr<Renderer> m_renderer;
		std::unique_ptr<WindowManager> m_windowManager;
		std::unique_ptr<ECS::ComponentManager> m_componentManager;
		std::unique_ptr<ECS::EntityManager> m_entityManager;
		std::unique_ptr<ECS::SystemManager> m_systemManager;
		std::shared_ptr<ECS::ParentingSystem> m_parentingSystem;
		std::shared_ptr<CameraSystem> m_cameraSystem;
	};

}
#endif
