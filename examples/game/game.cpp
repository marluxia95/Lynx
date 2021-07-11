#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <iostream>
#include <vector>
#include <glm/glm.hpp>

#include "Core/simpleGameEngine.h"
#include "Core/windowManager.h"
#include "Core/eventManager.h"

#include "Events/event.h"
#include "Events/keyEvent.h"
#include "Events/mouseEvent.h"

#include "Graphics/texture.h"
#include "Graphics/model.h"
#include "Graphics/shader.h"

#include "ECS/components.h"

using namespace Lynx;

// Initialize global variables

WindowManager gWindowManager;
EventManager gEventManager;
ResourceManager gResourceManager;
Editor gEditor;
Game game;

float camera_Speed_Multiplier = 0.0f;
bool firstMouse = true;
float lastX, lastY;
float sensitivity = 0.3f;
float mPitch, mYaw;
bool mouseActive = false;


void keyboard_input(const Event& ev)//KeyPressedEvent* ev)
{
	const KeyPressedEvent& key_event = static_cast<const KeyPressedEvent&>(ev);
	Entity camera = game.renderSystem->cameraEntity;
	auto transformComponent = game.GetComponent<Transform>(camera);
	auto cameraComponent = game.GetComponent<Camera>(camera);
	float cameraSpeed = 2.5f * game.delta_time * camera_Speed_Multiplier;

	if (key_event.m_keyCode == GLFW_KEY_W)
        transformComponent->position += cameraSpeed * transformComponent->rotation;
    if (key_event.m_keyCode == GLFW_KEY_S)
        transformComponent->position -= cameraSpeed * transformComponent->rotation;
    if (key_event.m_keyCode == GLFW_KEY_A)
        transformComponent->position -= glm::normalize(glm::cross(transformComponent->rotation, cameraComponent->up)) * cameraSpeed;
    if (key_event.m_keyCode == GLFW_KEY_D)
        transformComponent->position += glm::normalize(glm::cross(transformComponent->rotation, cameraComponent->up)) * cameraSpeed;
    if (key_event.m_keyCode == GLFW_KEY_LEFT_SHIFT){
    	camera_Speed_Multiplier = 3.0f;
    }else{
    	camera_Speed_Multiplier = 1.0f;
    }
	
}

void mouse_input(const Event& ev)
{
	const MouseCallbackEvent& mouse_event = static_cast<const MouseCallbackEvent&>(ev);
	Entity camera = game.renderSystem->cameraEntity;
	auto transformComponent = game.GetComponent<Transform>(camera);
	auto cameraComponent = game.GetComponent<Camera>(camera);
	double xpos = mouse_event.m_pos.x;
	double ypos = mouse_event.m_pos.y;

	if(!mouseActive)
		return;

	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	xoffset *= sensitivity;
	yoffset *= sensitivity;

	mPitch += yoffset;
	mYaw += xoffset;

	if(mPitch > 89.9f) 
		mPitch = 89.9f;

	if(mPitch < -89.9f)
		mPitch = -89.9f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	direction.y = sin(glm::radians(mPitch));
	direction.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	transformComponent->rotation = glm::normalize(direction);

}

void mouse_button_input(const Event& ev)
{
	const MouseButtonEvent& button_event = static_cast<const MouseButtonEvent&>(ev);
	if(button_event.m_keyCode == GLFW_MOUSE_BUTTON_2){
		mouseActive = true;
		glfwSetInputMode(gWindowManager.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}else{
		mouseActive = false;
		glfwSetInputMode(gWindowManager.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

void Update(const Event& ev)
{
	gEditor.Draw();
}

int main()
{

	// Initialize window in windowed mode
	gWindowManager.Init("Example", 1270, 720, false);

	// Enables the game's debug mode
	game.SetDebugMode(true);
	game.Init();
	log_set_level(LOG_DEBUG);

	Shader* shader = gResourceManager.LoadShader("Cube Shader", "res/shaders/standard/lighting.vs", "res/shaders/standard/lighting.fs");

	gEventManager.Subscribe(UpdateTick, Update);
	gEventManager.Subscribe(KeyPressed, keyboard_input);
	gEventManager.Subscribe(MousePosCallback, mouse_input);
	gEventManager.Subscribe(MouseKeyPressed, mouse_button_input);

	ModelLoader::loadModel("res/models/cube.obj", shader);

	// Runs the game
	game.Run();
	gWindowManager.Destroy();
	return 0;
}