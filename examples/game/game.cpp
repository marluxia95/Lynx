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
#include "Core/inputManager.h"

#include "Events/event.h"
#include "Events/keyEvent.h"
#include "Events/mouseEvent.h"
#include "Events/joystickEvent.h"

#include "Graphics/texture.h"
#include "Graphics/model.h"
#include "Graphics/shader.h"
#include "Graphics/cubemap.h"

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
char title[40];
Entity camera;

void movement()
{
	auto transformComponent = gApplication.GetComponent<Transform>(camera);
	auto cameraComponent = gApplication.GetComponent<Camera>(camera);

	float cameraSpeed = 20.5f * gApplication.delta_time * camera_Speed_Multiplier;

	if (Input::IsKeyDown(GLFW_KEY_W))
        transformComponent->position += cameraSpeed * transformComponent->rotation;
    if (Input::IsKeyDown(GLFW_KEY_S))
        transformComponent->position -= cameraSpeed * transformComponent->rotation;
    if (Input::IsKeyDown(GLFW_KEY_A))
        transformComponent->position -= glm::normalize(glm::cross(transformComponent->rotation, cameraComponent->up))* cameraSpeed;
    if (Input::IsKeyDown(GLFW_KEY_D))
        transformComponent->position += glm::normalize(glm::cross(transformComponent->rotation, cameraComponent->up))* cameraSpeed;
    if (Input::IsKeyDown(GLFW_KEY_LEFT_SHIFT))
    	camera_Speed_Multiplier = 5.0f;
    else
    	camera_Speed_Multiplier = 3.0f;
}

void mouse_input()
{
	auto transformComponent = gApplication.GetComponent<Transform>(camera);
	auto cameraComponent = gApplication.GetComponent<Camera>(camera);


	double xpos;
	double ypos;
	if(Input::IsJoystickConnected(GLFW_JOYSTICK_1)){
		xpos = (double)Input::GetJoyAxis(GLFW_JOYSTICK_1, 0) * 1000;
		ypos = (double)Input::GetJoyAxis(GLFW_JOYSTICK_1, 1) * 1000;
		log_debug("%f %f", xpos, ypos);
	}else{
		if(!mouseActive){
			firstMouse = true;
			return;
		}
		glm::dvec2 mpos = Input::GetMousePos();
		xpos = mpos.x;
		ypos = mpos.y;
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

void joystick_connected(const Event& ev)
{
	const JoystickConnectedEvent& joy_event = static_cast<const JoystickConnectedEvent&>(ev);

	log_info("Joystick connected : %s ( ID : %d )", joy_event.joystick->name, joy_event.joystick->id);
}

void joystick_disconnected(const Event& ev)
{
	const JoystickDisconnectedEvent& joy_event = static_cast<const JoystickDisconnectedEvent&>(ev);

	log_info("Joystick disconnected : %s", joy_event.joystick->name);
}

void Update(const Event& ev)
{
	snprintf(title,40 ,"Test game FPS : %d Errors : %d", (int)round(1/gApplication.delta_time), log_geterrorcount());
	glfwSetWindowTitle(gApplication.GetWindow(), title);
	mouse_input();
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
	gApplication.Init("Example", 800, 600, false);

	Shader* shader = gResourceManager.LoadShader("Cube Shader", "res/shaders/standard/lighting.vs", "res/shaders/standard/lighting.fs");

	EventManager::AddListener(UpdateTick, Update);
	EventManager::AddListener(MouseKeyPressed, mouse_button_input);
	EventManager::AddListener(JoystickConnected, joystick_connected);
	EventManager::AddListener(JoystickDisconnected, joystick_disconnected);

	Entity link = ModelLoader::loadModel("res/models/link_adult.obj", shader);

	{
		vector<Entity>* chl = getChildren(link);
		MeshRenderer* meshRenderer = gApplication.GetComponent<MeshRenderer>(chl->at(0));
		meshRenderer->ambient = glm::vec3(0.1f);
		meshRenderer->diffuse = glm::vec3(0.0f);
		meshRenderer->specular = glm::vec3(1.0f);
		meshRenderer->shininess = 64.0f;
		Texture* tex1 = gResourceManager.LoadTexture("container2","res/images/Link_grp.png");
		meshRenderer->texture_diffuse = tex1;

		gApplication.GetComponent<Transform>(chl->at(0))->scale = glm::vec3(0.1f);
		gApplication.GetComponent<Transform>(chl->at(0))->position = glm::vec3(20.0f);
	}

	Entity lightEnt = gApplication.CreateEntity("Light");
	gApplication.AddComponent(lightEnt, Transform{ glm::vec3(2,0,2), glm::vec3(0), glm::vec3(1) });
	gApplication.AddComponent(lightEnt, PointLight{ glm::vec3(0.4f, 0.7f , 0.4f ), glm::vec3(1.0f), glm::vec3(0.5f), 1.0f, 0.09f, 0.032f });
	
	auto directionalLight = gApplication.GetComponent<DirectionalLight>(gApplication.GetSystem<RenderSystem>()->directionalLight);
	directionalLight->direction = glm::vec3(-1.0f, 0.0f, 0.0f);
	directionalLight->ambient = glm::vec3(1.0f, 1.0f, 1.0f);
	directionalLight->diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	directionalLight->specular = glm::vec3(1.0f, 1.0f, 1.0f);
	directionalLight->intensity = 1.0f;


	// Setup skybox/cubemap
	std::vector<const char*> map_textures {
		"res/images/cubemap/right.jpg",
		"res/images/cubemap/left.jpg",
		"res/images/cubemap/top.jpg",
		"res/images/cubemap/bottom.jpg",
		"res/images/cubemap/front.jpg",
		"res/images/cubemap/back.jpg"
	};

	Cubemap* map = new Cubemap(&map_textures);
	gApplication.GetSystem<RenderSystem>()->SetCubemap(map);

	// Runs the gApplication
	gApplication.Run();
	delete map;
	return 0;
}
