#ifndef SIMPLEGAMEENGINE_H
#define SIMPLEGAMEENGINE_H

#include <stdio.h>
#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include "renderer.h"
#include "logger.h"

enum GameState{
	GAME_PAUSE,
	GAME_ACTIVE
};

class Game {
public:
	Game(unsigned int width, unsigned int height);
	~Game();
	void Run();
	GameState State;
	bool keys[1024];
	unsigned int WINDOW_WIDTH = 1280;
	unsigned int WINDOW_HEIGHT = 720;
	Renderer renderer;
	Logger logger;
	GLFWwindow* window;

private:
	bool running;
	bool debugMode = false;

	float delta_time = 0.0f;
	float last_FrameTime = 0.0f;
	float camera_Speed_Multiplier = 1.0f;

	
	char* windowName = "Simple Game Engine";

	void initWindow();


	//void MouseCallback(GLFWwindow* window, double xpos, double ypos);
	void ProcessInput(GLFWwindow *window);
	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	//void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

	void DebugWindow();

	virtual void OnInit();
	virtual void OnUpdate();
	virtual void OnRender();
	virtual void OnLast();


};

#endif