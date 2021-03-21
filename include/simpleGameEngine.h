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
#include "renderer.h"
#include "camera.h"
#include "logger.h"
#include "scene.h"
#include "resourceManager.h"

namespace Lynx{

	enum GameState{
		GAME_PAUSE,
		GAME_ACTIVE
	};

	class Game {
	public:
		Game(unsigned int width, unsigned int height);
		~Game();
		void Run();
		int CreateScene(const char* name);
		int BindScene(Scene* scene);
		bool SetActiveScene(int id);
		void SetDebugMode(bool mode);
		Scene* GetActiveScene();
		GameState State;
		
		unsigned int WINDOW_WIDTH = 1280;
		unsigned int WINDOW_HEIGHT = 720;
		Renderer renderer;
		ResourceManager resourceManager;
		Logger logger;
		GLFWwindow* window;
		static int activeScene;
		


	private:
		bool running;
		static bool debugMode;

		float delta_time = 0.0f;
		float last_FrameTime = 0.0f;
		float camera_Speed_Multiplier = 1.0f;

		
		char* windowName = "Simple Game Engine";

		
		static std::vector<Scene*> Scenes;

		
		
		static bool mouseLock;
		static int polygonMode;
		static bool keys[1024];

		void initWindow();


		static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
		static void ProcessInput(GLFWwindow *window);
		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

		void DebugWindow();

		virtual void OnInit();
		virtual void OnUpdate();
		virtual void OnRender();
		virtual void OnLast();


	};

}
#endif