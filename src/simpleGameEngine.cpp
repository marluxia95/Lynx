#include <stdio.h>
#include <math.h>
#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include "renderer.h"
#include "logger.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "simpleGameEngine.h"

namespace Lynx {

bool Game::mouseLock;
int Game::polygonMode;
bool Game::keys[1024];

Game::Game(unsigned int width, unsigned int height):
	logger("main.log", LOG_INFO, false)
{

	WINDOW_WIDTH = width;
	WINDOW_HEIGHT = height;

	initWindow();

}


Game::~Game(){
	OnLast();
	ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
	glfwTerminate();
}


void Game::initWindow(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
  
  	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, windowName, NULL, NULL);
	if (window == NULL)
	{
	    printf("Failed to create window\n");
	    glfwTerminate();
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	//glfwSetUserPointer(game_, this);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);  
	glfwSetCursorPosCallback(window, MouseCallback);  
	glfwSetKeyCallback(window, KeyCallback);

	bool err = glewInit() != GLEW_OK;   

	glEnable(GL_DEPTH_TEST);

	IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    OnInit();

}

void Game::Run(){
	while(!glfwWindowShouldClose(window)|running)
	{
		ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        float current_FrameTime = glfwGetTime();
		delta_time = current_FrameTime - last_FrameTime;
		last_FrameTime = current_FrameTime;
		glfwPollEvents();

		OnUpdate();

		//ProcessInput();

        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if(debugMode){
			DebugWindow();
		}

		OnRender();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	    glfwSwapBuffers(window);  
	    glfwPollEvents();

    }
}


void Game::MouseCallback(GLFWwindow* window, double xpos, double ypos){/*
	if(!lockMouse){return;}
	if (firstMouse) // initially set to true
	{
	    lastX = xpos;
	    lastY = ypos;
	    firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	const float sensitivity = 0.25f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	

	yaw   += xoffset;
	pitch += yoffset;  

	if(pitch > 89.0f)
	  pitch =  89.0f;
	if(pitch < -89.0f)
	  pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	camera.front = glm::normalize(direction);*/
}

void Game::ProcessInput(GLFWwindow *window)
{	
	/*
	
	float cameraSpeed = 2.5f * delta_time * camera_Speed_Multiplier;
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.pos += cameraSpeed * camera.front;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.pos -= cameraSpeed * camera.front;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.pos -= glm::normalize(glm::cross(camera.front, camera.up)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.pos += glm::normalize(glm::cross(camera.front, camera.up)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
    	camera_Speed_Multiplier = 3.0f;
    }else{
    	camera_Speed_Multiplier = 1.0f;
    }
    */

    	
    	
}

void Game::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	
    if (key ==  GLFW_KEY_I && action == GLFW_PRESS){
        if(polygonMode<1){polygonMode++;}else{polygonMode = 0;}
    	//glPolygonMode(GL_FRONT_AND_BACK, polygonModes[polygonMode]);

    }
    if(key == GLFW_KEY_L && action == GLFW_PRESS){
    	if(mouseLock){
    		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    		mouseLock = false;
    	}else{
    		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    		mouseLock = true;
    	}
    }

    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    	glfwSetWindowShouldClose(window, true);

    if(key >= 0 && key < 1024){
    	if(action == GLFW_PRESS)
    		keys[key] = true;
    	else if(action == GLFW_RELEASE)
    		keys[key] = false;

    }


}

void Game::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}  

void Game::DebugWindow(){
	ImGui::Begin("Lightning example");  
	ImGui::Text("FPS : %f", round(1/delta_time));
	ImGui::End();
}

}
