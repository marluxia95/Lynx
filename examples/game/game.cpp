#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <iostream>
#include <vector>
#include <glm/glm.hpp>

#include "lynx.h"

#include "Systems/renderSystem.h"
#include "Systems/cameraSystem.h"
#include "Graphics/cubemap.h"
#include "Graphics/terrain.h"

using namespace Lynx;

float camera_Speed_Multiplier = 0.0f;
bool firstMouse = true;
float lastX, lastY;
float sensitivity = 0.3f;
float mPitch, mYaw;
bool mouseActive = false;
char title[40];
Entity camera;
Scene* scene;
GameApplication* applicationInstance;

void movement()
{
	Lynx::GameApplication* applicationInstance = GameApplication::GetGameInstance();
	auto transformComponent = scene->GetComponent<Transform>(camera);
	auto cameraComponent = scene->GetComponent<Camera>(camera);

	float cameraSpeed = 20.5f * applicationInstance->delta_time * camera_Speed_Multiplier;

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

	if(Input::IsJoystickConnected(GLFW_JOYSTICK_1)){
		transformComponent->position += cameraSpeed * transformComponent->rotation * Input::GetJoyAxis(GLFW_JOYSTICK_1, 4);
		transformComponent->position += cameraSpeed * transformComponent->rotation * Input::GetJoyAxis(GLFW_JOYSTICK_1, 3);
	}
}

void mouse_input()
{
	auto transformComponent = scene->GetComponent<Transform>(camera);
	auto cameraComponent = scene->GetComponent<Camera>(camera);


	double xpos;
	double ypos;
	if(Input::IsJoystickConnected(GLFW_JOYSTICK_1)){
		mYaw += Input::GetJoyAxis(GLFW_JOYSTICK_1, 0);
		mPitch += Input::GetJoyAxis(GLFW_JOYSTICK_1, 1);
	}else{
		if(!mouseActive){
			firstMouse = true;
			return;
		}
		glm::dvec2 mpos = Input::GetMousePos();
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

void mouse_button_input(const Event& ev)
{
	const MouseButtonEvent& button_event = static_cast<const MouseButtonEvent&>(ev);
	if(button_event.m_keyCode == GLFW_MOUSE_BUTTON_2 && button_event.m_action == GLFW_PRESS){
		mouseActive = true;
		glfwSetInputMode(GameApplication::GetGameInstance()->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}else if(button_event.m_action == GLFW_RELEASE){
		mouseActive = false;
		glfwSetInputMode(GameApplication::GetGameInstance()->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
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
	snprintf(title,40 ,"Test game FPS : %d Errors : %d", (int)round(1/applicationInstance->delta_time), log_geterrorcount());
	glfwSetWindowTitle(applicationInstance->GetWindow(), title);
	mouse_input();
	movement();
}

vector<Entity>* getChildren(Entity parent)
{
	vector<Entity>* ents = new vector<Entity>();
	LYNX_ASSERT(parent != NULL, "Parent must be a valid entity !");
	for(int e = 0; e < scene->GetEntityCount(); e++){
		if(scene->GetComponent<Parent>(e)->parentEntity == parent){
			ents->push_back(e);
		}
	}

	return ents;
}

int main()
{
	// Enables the gApplication's debug mode
	log_set_level(LOG_DEBUG);

	log_debug("Adding initial events...");

	applicationInstance = new GameApplication();
	scene = applicationInstance->CreateScene();

	auto resourceManager = applicationInstance->GetResourceManager();

	applicationInstance->LoadDefaultComponents();
	applicationInstance->LoadDefaultSystems();

	applicationInstance->RegisterSystem<RenderSystem>();
	{
		Signature signature;
		signature.set(applicationInstance->GetComponentType<Transform>());
		signature.set(applicationInstance->GetComponentType<MeshRenderer>());
		applicationInstance->SetSystemSignature<RenderSystem>(signature);
	}

	EventManager::AddListener(UpdateTick, Update);
	EventManager::AddListener(MouseKeyPressed, mouse_button_input);
	EventManager::AddListener(JoystickConnected, joystick_connected);
	EventManager::AddListener(JoystickDisconnected, joystick_disconnected);

	// Initialize window in windowed mode
	applicationInstance->Init("Example", 1920, 1080, false);

	log_debug("Loading resources");
	Graphics::Shader* shader = resourceManager->LoadShader("res/shaders/standard/lighting.shader");

	Entity link = ModelLoader::loadModel(scene, "res/models/link_adult.obj", shader);
	{
		vector<Entity>* chl = getChildren(link);
		MeshRenderer* meshRenderer = scene->GetComponent<MeshRenderer>(chl->at(0));
		meshRenderer->ambient = glm::vec3(0.1f);
		meshRenderer->diffuse = glm::vec3(0.0f);
		meshRenderer->specular = glm::vec3(1.0f);
		meshRenderer->shininess = 64.0f;
		Graphics::Texture tex1 = resourceManager->LoadTexture("res/images/Link_grp.png");
		meshRenderer->texture_diffuse = tex1;

		scene->GetComponent<Transform>(chl->at(0))->scale = glm::vec3(0.1f);
		scene->GetComponent<Transform>(chl->at(0))->position = glm::vec3(20.0f);
		delete chl;
	}

	Entity lightEnt = scene->CreateEntity("Light");
	scene->AddComponent(lightEnt, Transform{ glm::vec3(2,0,2), glm::vec3(0), glm::vec3(1) });
	scene->AddComponent(lightEnt, PointLight{ glm::vec3(0.4f, 0.7f , 0.4f ), glm::vec3(1.0f), glm::vec3(0.5f), 1.0f, 0.09f, 0.032f });
	
	auto directionalLight = scene->GetComponent<DirectionalLight>(applicationInstance->GetSystem<RenderSystem>()->directionalLight);
	directionalLight->direction = glm::vec3(-1.0f, 0.0f, 0.0f);
	directionalLight->ambient = glm::vec3(1.0f, 1.0f, 1.0f);
	directionalLight->diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	directionalLight->specular = glm::vec3(1.0f, 1.0f, 1.0f);
	directionalLight->intensity = 1.0f;

	/*

	Entity terrain = gApplication.CreateEntity("Terrain");
	Terrain* terr_mesh = new Terrain(load_heightmap_from_image("res/images/testheightmap.jpg"));
	gApplication.AddComponent<MeshRenderer>(terrain, MeshRenderer{glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f), 0.0f, terr_mesh, shader});

	*/

	
	// Setup skybox/cubemap
	std::vector<const char*> map_textures {
		"res/images/cubemap/right.jpg",
		"res/images/cubemap/left.jpg",
		"res/images/cubemap/top.jpg",
		"res/images/cubemap/bottom.jpg",
		"res/images/cubemap/front.jpg",
		"res/images/cubemap/back.jpg"
	};

	Graphics::Cubemap* map = new Graphics::Cubemap();
	map->Load(&map_textures);
	applicationInstance->GetSystem<RenderSystem>()->SetCubemap(map);	
	

	// Runs the gApplication
	applicationInstance->Run();
	//delete map;
	return 0;
}
