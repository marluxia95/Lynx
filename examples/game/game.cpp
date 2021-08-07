#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <iostream>
#include <vector>
#include <glm/glm.hpp>

#include "Core/ECS/components.h"
#include "Core/simpleGameEngine.h"
#include "Core/windowManager.h"
#include "Core/eventManager.h"

#include "Events/event.h"
#include "Events/keyEvent.h"
#include "Events/mouseEvent.h"

#include "Graphics/texture.h"
#include "Graphics/model.h"
#include "Graphics/shader.h"

#include "Systems/renderSystem.h"

using namespace Lynx;

// Initialize global variables
ResourceManager gResourceManager;
Application gApplication;

float camera_Speed_Multiplier = 0.0f;
bool firstMouse = true;
float lastX, lastY;
float sensitivity = 0.3f;
float mPitch, mYaw;
bool mouseActive = false;
bool keys[1024];
Entity camera;

void movement()
{
	auto transformComponent = gApplication.GetComponent<Transform>(camera);
	auto cameraComponent = gApplication.GetComponent<Camera>(camera);

	float cameraSpeed = 2.5f * gApplication.delta_time * camera_Speed_Multiplier;

	if (keys[GLFW_KEY_W])
        transformComponent->position += cameraSpeed * transformComponent->rotation;
    if (keys[GLFW_KEY_S])
        transformComponent->position -= cameraSpeed * transformComponent->rotation;
    if (keys[GLFW_KEY_A])
        transformComponent->position -= glm::normalize(glm::cross(transformComponent->rotation, cameraComponent->GetUpVector())) * cameraSpeed;
    if (keys[GLFW_KEY_D])
        transformComponent->position += glm::normalize(glm::cross(transformComponent->rotation, cameraComponent->GetUpVector())) * cameraSpeed;
    if (keys[GLFW_KEY_LEFT_SHIFT]){
    	camera_Speed_Multiplier = 3.0f;
    }else{
    	camera_Speed_Multiplier = 1.0f;
    }
}

void keyboard_input(const Event& ev)//KeyPressedEvent* ev)
{
	const KeyPressedEvent& key_event = static_cast<const KeyPressedEvent&>(ev);
	if(key_event.m_keyCode > 1024)
		return;

	if(key_event.m_action == GLFW_PRESS)
		keys[key_event.m_keyCode] = true;
	else if(key_event.m_action == GLFW_RELEASE)
		keys[key_event.m_keyCode] = false;
}

void mouse_input(const Event& ev)
{
	const MouseCallbackEvent& mouse_event = static_cast<const MouseCallbackEvent&>(ev);
	auto transformComponent = gApplication.GetComponent<Transform>(camera);
	auto cameraComponent = gApplication.GetComponent<Camera>(camera);
	double xpos = mouse_event.m_pos.x;
	double ypos = mouse_event.m_pos.y;

	if(!mouseActive){
		firstMouse = true;
		return;
	}

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
	if(button_event.m_keyCode == GLFW_MOUSE_BUTTON_2 && button_event.m_action == GLFW_PRESS){
		mouseActive = true;
		glfwSetInputMode(gApplication.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}else if(button_event.m_action == GLFW_RELEASE){
		mouseActive = false;
		glfwSetInputMode(gApplication.GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

void Update(const Event& ev)
{
	movement();
}

vector<Entity>* getChildren(Entity parent)
{
	vector<Entity>* ents = new vector<Entity>();
	for(int e = 0; e < gApplication.GetEntityCount(); e++){
		if(gApplication.GetComponent<Parent>(e)->parentEntity == parent){
			ents->push_back(e);
		}
	}

	return ents;
}

int main()
{
	// Enables the gApplication's debug mode
	log_set_level(LOG_DEBUG);

	EventManager::AddListener(EngineInit, [](const Event& ev) {
		gApplication.LoadDefaultComponents();
		gApplication.LoadDefaultSystems();
	});

	// Initialize window in windowed mode
	gApplication.Init("Example", 1270, 720, false);

	auto renderSystem = gApplication.GetSystem<RenderSystem>();
	camera = renderSystem->GetMainCamera();


	Shader* shader = gResourceManager.LoadShader("Cube Shader", "res/shaders/standard/lighting.vs", "res/shaders/standard/lighting.fs");
	Shader* shader2 = gResourceManager.LoadShader("Light Shader", "res/shaders/standard/standard.vs", "res/shaders/standard/standard.fs");

	EventManager::AddListener(UpdateTick, Update);
	EventManager::AddListener(KeyPressed, keyboard_input);
	EventManager::AddListener(MousePosCallback, mouse_input);
	EventManager::AddListener(MouseKeyPressed, mouse_button_input);

	Entity cube = ModelLoader::loadModel("res/models/monkey.obj", shader);

	/*
	vector<Entity>* chl = getChildren(cube);
	MeshRenderer* meshRenderer = gApplication.GetComponent<MeshRenderer>(chl->at(0));
	meshRenderer->ambient = glm::vec3(1.0f);
	meshRenderer->diffuse = glm::vec3(1.0f);
	meshRenderer->specular = glm::vec3(1.0f);
	meshRenderer->shininess = 256.0f;

	Entity lightEnt = gApplication.CreateEntity("Light");
	gApplication.AddComponent(lightEnt, Transform{ glm::vec3(2,0,2), glm::vec3(0), glm::vec3(1) });
	gApplication.AddComponent(lightEnt, PointLight{ glm::vec3(0.4f, 0.7f , 0.4f ), glm::vec3(1.0f), glm::vec3(0.5f), 1.0f, 0.09f, 0.032f });
	
	auto directionalLight = gApplication.GetComponent<DirectionalLight>(renderSystem->GetDirectionalLight());
	directionalLight->direction = glm::vec3(1.0f, 1.0f, 0.0f);
	*/
	// Runs the gApplication
	gApplication.Run();
	return 0;
}