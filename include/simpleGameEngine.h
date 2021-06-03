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
#include "entityManager.h"
#include "componentManager.h"
#include "systemManager.h"
#include "entity.h"
#include "camera.h"
#include "model.h"
#include "object.h"
#include "scene.h"
#include "mesh.h"
#include "resourceManager.h"

namespace Lynx::Core {

	enum GameState{
		GAME_PAUSE,
		GAME_ACTIVE
	};

	class Game {
	public:
		Game(unsigned int width, unsigned int height);
		~Game();

		void Run();
		//int CreateScene(const char* name);
		//int BindScene(Scene* scene);
		//bool SetActiveScene(int id);
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
		T& GetComponent(Entity entity){
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

		//Scene* GetActiveScene();
		GameState State;
		
		unsigned int WINDOW_WIDTH = 1280;
		unsigned int WINDOW_HEIGHT = 720;

		ResourceManager resourceManager;
		GLFWwindow* window;

		static bool keys[1024];
		static bool mouseLock;
		static double mouseXPos, mouseYPos;
		float delta_time = 0.0f;
		float last_FrameTime = 0.0f;
	private:
		bool running;
		static bool debugMode;

	

		
		char* windowName = "Simple Game Engine";

		// debug variables
		int selectedId; 
		Shader* selectedShader;
	    const char* selectedName;
	    const char* buttonName;
		
		
		static int polygonMode;
		

		static float pitch;
		static float yaw;
		static float lastX;
		static float lastY;

		static bool firstMouse;
		int debug_id;
		void initWindow();

		static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

		

		std::unique_ptr<ECS::ComponentManager> componentManager;
		std::unique_ptr<ECS::EntityManager> entityManager;
		std::unique_ptr<ECS::SystemManager> systemManager;

		void DebugWindow();
		void InspectorWindow();
		void DebugOverlay();
		bool inspectorToggle;
		bool overlayToggle;

		virtual void OnInit();
		virtual void OnUpdate();
		virtual void OnRender();
		virtual void OnLast();


	};

}
#endif