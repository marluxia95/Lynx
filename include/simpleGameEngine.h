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
#include "camera.h"
#include "logger.h"
#include "scene.h"
#include "mesh.h"
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
		ResourceManager resourceManager;
		Logger logger;
		GLFWwindow* window;
		static int activeScene;
		static bool keys[1024];
		static bool mouseLock;
		static double mouseXPos, mouseYPos;
		float delta_time = 0.0f;
		float last_FrameTime = 0.0f;
		


	private:
		bool running;
		static bool debugMode;

	

		
		char* windowName = "Simple Game Engine";

		
		static std::vector<Scene*> Scenes;

		// debug variables
		int selectedType; // 1 for sprite 2 for camera
	    Camera* selectedCamera;
		Mesh3D* selectedMesh3D;
	    Sprite* selectedSprite;
		Shader* selectedShader;
	    const char* selectedName;
	    const char* buttonName;
		
		
		static int polygonMode;
		

		static float pitch;
		static float yaw;
		static float lastX;
		static float lastY;

		static bool firstMouse;

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