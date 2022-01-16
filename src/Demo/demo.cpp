#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <iostream>
#include <vector>
#include <glm/glm.hpp>

#include "lynx.h"
#include "Systems/renderSystem.h"
#include "Systems/cameraSystem.h"
#include "Scripting/luaRuntime.h"
#include "Graphics/cubemap.h"

#include "Physics/physicsObject.h"

float camera_Speed_Multiplier = 1.0f;
bool firstMouse = true;
float lastX, lastY;
float sensitivity = 0.1f;
float mPitch, mYaw;
bool mouseActive = false;
char title[40];

Lynx::EntityID camera;
Lynx::Scene* scene;
Lynx::GameApplication* applicationInstance;

void movement()
{
	Lynx::GameApplication* applicationInstance = Lynx::GameApplication::GetGameInstance();
	auto transformComponent = scene->GetComponent<Lynx::Transform>(camera);
	auto cameraComponent = scene->GetComponent<Lynx::Camera>(camera);

	float cameraSpeed = applicationInstance->GetDeltaTime() * camera_Speed_Multiplier;

	if (Lynx::Input::IsKeyDown(GLFW_KEY_W))
        transformComponent->position += cameraSpeed * transformComponent->rotation;
    if (Lynx::Input::IsKeyDown(GLFW_KEY_S))
        transformComponent->position -= cameraSpeed * transformComponent->rotation;
    if (Lynx::Input::IsKeyDown(GLFW_KEY_A))
        transformComponent->position -= glm::normalize(glm::cross(transformComponent->rotation, cameraComponent->up))* cameraSpeed;
    if (Lynx::Input::IsKeyDown(GLFW_KEY_D))
        transformComponent->position += glm::normalize(glm::cross(transformComponent->rotation, cameraComponent->up))* cameraSpeed;
    if (Lynx::Input::IsKeyDown(GLFW_KEY_LEFT_SHIFT))
    	camera_Speed_Multiplier = 5.0f;
    else
    	camera_Speed_Multiplier = 3.0f;

	if(Lynx::Input::IsJoystickConnected(GLFW_JOYSTICK_1)){
		transformComponent->position += cameraSpeed * transformComponent->rotation * Lynx::Input::GetJoyAxis(GLFW_JOYSTICK_1, 4);
		transformComponent->position += cameraSpeed * transformComponent->rotation * Lynx::Input::GetJoyAxis(GLFW_JOYSTICK_1, 3);
	}
}

void mouse_input()
{
	auto transformComponent = scene->GetComponent<Lynx::Transform>(camera);
	auto cameraComponent = scene->GetComponent<Lynx::Camera>(camera);


	float xpos;
	float ypos;
	if(Lynx::Input::IsJoystickConnected(GLFW_JOYSTICK_1)){
		mYaw += Lynx::Input::GetJoyAxis(GLFW_JOYSTICK_1, 0);
		mPitch += Lynx::Input::GetJoyAxis(GLFW_JOYSTICK_1, 1);
	}else{
		if(!mouseActive){
			firstMouse = true;
			return;
		}
		glm::vec2 mpos = Lynx::Input::GetMousePos();
		xpos = mpos.x;
		ypos = mpos.y;

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
	}

	

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

int mouse_button_input(const Lynx::Event& ev)
{
	const Lynx::MouseButtonEvent& button_event = static_cast<const Lynx::MouseButtonEvent&>(ev);
	if(button_event.m_keyCode == GLFW_MOUSE_BUTTON_2 && button_event.m_action == GLFW_PRESS){
		mouseActive = true;
		glfwSetInputMode(Lynx::GameApplication::GetGameInstance()->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}else if(button_event.m_action == GLFW_RELEASE){
		mouseActive = false;
		glfwSetInputMode(Lynx::GameApplication::GetGameInstance()->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	return 1;
}

int joystick_connected(const Lynx::Event& ev)
{
	const Lynx::JoystickConnectedEvent& joy_event = static_cast<const Lynx::JoystickConnectedEvent&>(ev);

	log_info("Joystick connected : %s ( ID : %d )", joy_event.joystick->name, joy_event.joystick->id);
	
	return 1;
}

int joystick_disconnected(const Lynx::Event& ev)
{
	const Lynx::JoystickDisconnectedEvent& joy_event = static_cast<const Lynx::JoystickDisconnectedEvent&>(ev);

	log_info("Joystick disconnected : %s", joy_event.joystick->name);

	return 1;
}

int Update(const Lynx::Event& ev)
{
	//snprintf(title,40 ,"Test game FPS : %d Errors : %d", (int)round(1/applicationInstance->GetDeltaTime()), log_geterrorcount());
	//glfwSetWindowTitle(applicationInstance->GetWindow(), title);
	mouse_input();
	movement();
	
	return 1;
}

std::vector<Lynx::EntityID>* getChildren(Lynx::EntityID parent)
{
	std::vector<Lynx::EntityID>* ents = new std::vector<Lynx::EntityID>();
	LYNX_ASSERT(parent != NULL, "Parent must be a valid entity !");
	for(int e = 0; e < scene->GetEntityCount(); e++){
		if(scene->GetComponent<Lynx::Parent>(e)->parentEntity == parent){
			ents->push_back(e);
		}
	}

	return ents;
}

int main(int argc, char** argv)
{
	// Enables the gApplication's debug mode
	for (int i = 1; i < argc; ++i) {
		if(std::string(argv[i]) == "--debug")
			log_set_level(LOG_DEBUG);
	}

	log_debug("Adding initial events...");

	applicationInstance = new Lynx::GameApplication();

	scene = applicationInstance->CreateScene();

	auto resourceManager = applicationInstance->GetResourceManager();

	applicationInstance->LoadDefaultComponents();
	applicationInstance->LoadDefaultSystems();


	applicationInstance->RegisterSystem<Lynx::RenderSystem>();
	{
		Lynx::Signature signature;
		signature.set(applicationInstance->GetComponentType<Lynx::Transform>());
		signature.set(applicationInstance->GetComponentType<Lynx::MeshRenderer>());
		applicationInstance->SetSystemSignature<Lynx::RenderSystem>(signature);
	}

	applicationInstance->RegisterSystem<Lynx::Lua::LuaRuntime>();
	{
		Lynx::Signature signature;
		signature.set(applicationInstance->GetComponentType<Lynx::LuaScript>());
		applicationInstance->SetSystemSignature<Lynx::Lua::LuaRuntime>(signature);
	}

	Lynx::EventManager::AddListener(Lynx::UpdateTick, Update);
	Lynx::EventManager::AddListener(Lynx::MouseKeyPressed, mouse_button_input);
	Lynx::EventManager::AddListener(Lynx::JoystickConnected, joystick_connected);
	Lynx::EventManager::AddListener(Lynx::JoystickDisconnected, joystick_disconnected);

	//auto module = Lynx::ModuleManager::LoadEngineModule("LynxPhysics");
	//module->Init();

	log_info("Initializing window");

	// Initialize window in windowed mode
	applicationInstance->Init("Example", 1920, 1080, false);

	log_debug("Loading resources");
	std::shared_ptr<Lynx::Graphics::Shader> shader = resourceManager->LoadShader("res/shaders/standard/lighting.vert", "res/shaders/standard/lighting.frag");

	log_info("Adding scene objects");

	Lynx::EntityID cube = Lynx::ModelLoader::loadModel(scene, "res/models/cube.fbx", shader);
	{
		Lynx::MeshRenderer* meshRenderer = scene->GetComponent<Lynx::MeshRenderer>(cube);
		meshRenderer->ambient = glm::vec3(0.1f);
		meshRenderer->diffuse = glm::vec3(0.0f);
		meshRenderer->specular = glm::vec3(0.1f);
		meshRenderer->shininess = 24.0f;
		meshRenderer->texture_diffuse = resourceManager->LoadTexture("res/textures/box.dds");
	}

	Lynx::EntityID lightEnt = scene->CreateEntity("Light");
	scene->AddComponent(lightEnt, Lynx::Transform{ glm::vec3(2,0,2), glm::vec3(0), glm::vec3(1) });
	scene->AddComponent(lightEnt, Lynx::PointLight{ glm::vec3(0.4f, 0.7f , 0.4f ), glm::vec3(1.0f), glm::vec3(0.5f), 1.0f, 0.09f, 0.032f });
	
	auto directionalLight = scene->GetComponent<Lynx::DirectionalLight>(applicationInstance->GetSystem<Lynx::RenderSystem>()->directionalLight);
	directionalLight->direction = glm::vec3(-1.0f, 0.0f, 0.0f);
	directionalLight->ambient = glm::vec3(1.0f, 1.0f, 1.0f);
	directionalLight->diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	directionalLight->specular = glm::vec3(1.0f, 1.0f, 1.0f);
	directionalLight->intensity = 1.0f;

	log_info("Loading cubemap");
	
	// Setup skybox/cubemap
	/*
	std::vector<const char*> map_textures {
		"res/images/cubemap/right.jpg",
		"res/images/cubemap/left.jpg",
		"res/images/cubemap/top.jpg",
		"res/images/cubemap/bottom.jpg",
		"res/images/cubemap/front.jpg",
		"res/images/cubemap/back.jpg"
	};
	Lynx::Graphics::Cubemap* cubemap = new Lynx::Graphics::Cubemap();
	cubemap->Load(&map_textures);
	applicationInstance->GetSystem<Lynx::RenderSystem>()->SetCubemap(cubemap);*/

	log_info("Application fully started");
	
	// Runs the gApplication
	applicationInstance->SetApplicationState(Lynx::STATE_ACTIVE);
	applicationInstance->Run();

	//delete cubemap;
	return 0;
}
