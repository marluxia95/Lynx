#include <stdio.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "simpleGameEngine.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "mesh.h"

#include "lightingSystem.h"
#include "parentingSystem.h"
#include "systemManager.h"
#include "cameraSystem.h"
#include "renderSystem.h"
#include "components.h"

extern "C" {
    #include "logger.h"
}

namespace Lynx::Core {

bool Game::mouseLock;
int Game::polygonMode;
bool Game::keys[1024];
bool Game::debugMode;
float Game::pitch;
float Game::yaw;
float Game::lastX;
float Game::lastY;
double Game::mouseXPos, Game::mouseYPos;
bool Game::firstMouse;

Game::Game(unsigned int width, unsigned int height):
    resourceManager(), editor()
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

void Game::SetDebugMode(bool mode){
	debugMode = mode;
}

void Game::initWindow(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
  
  	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, windowName, NULL, NULL);
	if (window == NULL)
	{
        log_fatal("Failed to create window");
	    glfwTerminate();
		exit(1);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Limit FPS
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);  
	glfwSetCursorPosCallback(window, MouseCallback);  
	glfwSetKeyCallback(window, KeyCallback);

    mouseLock = false;

	bool err = glewInit() != GLEW_OK;   

    if(err){
    	log_fatal("Failed to initalize GLEW");
		exit(1);
    }

	glEnable(GL_DEPTH_TEST);

    // Enable face culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);  
    glFrontFace(GL_CW);  


	IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    componentManager = std::make_unique<ECS::ComponentManager>();
    entityManager = std::make_unique<ECS::EntityManager>();
    systemManager = std::make_unique<ECS::SystemManager>();

    RegisterComponent<const char*>();
    RegisterComponent<Transform>();
    RegisterComponent<RigidBody>();
    RegisterComponent<Generic>();
    RegisterComponent<MeshRenderer>();
    RegisterComponent<Camera>();
	RegisterComponent<Parent>();
	RegisterComponent<PointLight>();

    renderSystem = RegisterSystem<RenderSystem>();
    {
    	Signature signature;
    	signature.set(GetComponentType<Transform>());
    	signature.set(GetComponentType<MeshRenderer>());
    	SetSystemSignature<RenderSystem>(signature);
    }

    cameraSystem = RegisterSystem<CameraSystem>();
    {
    	Signature signature;
    	signature.set(GetComponentType<Transform>());
    	signature.set(GetComponentType<Camera>());
    	SetSystemSignature<CameraSystem>(signature);
    }

    physicsSystem = RegisterSystem<PhysicsSystem>();
    {
    	Signature signature;
    	signature.set(GetComponentType<Transform>());
    	signature.set(GetComponentType<RigidBody>());
    	SetSystemSignature<PhysicsSystem>(signature);
    }

    parentingSystem = RegisterSystem<ECS::ParentingSystem>();
	{        	
		Signature signature;
       	signature.set(GetComponentType<Transform>());
    	signature.set(GetComponentType<Parent>());
    	SetSystemSignature<ECS::ParentingSystem>(signature);
    }

	lightingSystem = RegisterSystem<LightingSystem>();
	{
		Signature signature;
		signature.set(GetComponentType<Transform>());
		signature.set(GetComponentType<PointLight>());
		SetSystemSignature<LightingSystem>(signature);
	}


    renderSystem->Init();
    cameraSystem->Init();
    physicsSystem->Init();
	editor.Init();
    OnInit();
}


/*
*
*   Entity Component System
* 
*/



Entity Game::CreateEntity() {
    return entityManager->CreateEntity();
}


Entity Game::CreateEntity(const char* name) {
    Entity newEnt = entityManager->CreateEntity();
    log_debug("Created new entity %d", newEnt);
    AddComponent(newEnt, Generic{name=name});
    return newEnt;
}


void Game::DestroyEntity(Entity entity) {
    entityManager->DestroyEntity(entity);
}


/*
*   End of ECS
*/

// Main Loop

void Game::Run(){
	do
	{
        float current_FrameTime = glfwGetTime();
		delta_time = current_FrameTime - last_FrameTime;
		last_FrameTime = current_FrameTime;
        glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
		
		OnUpdate();
        	
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if(debugMode){
			editor.Draw();
		}

		// This needs to be improved
        parentingSystem->Update();
        renderSystem->Update();
        cameraSystem->Update();
        physicsSystem->Update();
        
		OnRender();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	    glfwSwapBuffers(window);  
	    glfwPollEvents();

    } while((!glfwWindowShouldClose(window))|running);
}

// Game Input

void Game::MouseCallback(GLFWwindow* window, double xpos, double ypos){
    	mouseXPos = xpos;
    	mouseYPos = ypos;
}

void Game::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    	glfwSetWindowShouldClose(window, true);



    if(key >= 0 && key < 1024){
    	if(action == GLFW_PRESS)
    		keys[key] = true;
    	else if(action == GLFW_RELEASE)
    		keys[key] = false;

    }

    if(keys[GLFW_KEY_LEFT_SHIFT]&&keys[GLFW_KEY_LEFT_CONTROL]&&keys[GLFW_KEY_F1])
    	if(debugMode){debugMode = false;}else{debugMode = true;}
    

    if(keys[GLFW_KEY_LEFT_SHIFT]&&keys[GLFW_KEY_LEFT_CONTROL]&&keys[GLFW_KEY_F2]){
    	if(mouseLock){
    		mouseLock = false; 
    		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    	}else{
    		mouseLock = true; 
    		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    	}
    }


}

void Game::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}  

}
