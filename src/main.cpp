#include <stdio.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include "simpleGameEngine.h"
#include "texture.h"
#include "model.h"
#include "light.h"
#include "components.h"

using namespace Lynx;

// Initialize global variables
Core::Game game(1280,720);

//Camera camera(CAMERA_PERSPECTIVE,1280, 720);

vector<Vertex> cube_vertices = {
    {
        vec3(-1.0f, -1.0f, -1.0f),
        vec3(0.0f, 0.0f,  1.0f),
        vec3(0.0f, 0.0f, -1.0f)
    },

    {
        vec3(1.0f, -1.0f, -1.0f),
        vec3(1.0f, 0.0f, 0.0f),
        vec3(0.0f, 0.0f, -1.0f)

    },
    {
        vec3(1.0f, 1.0f, -1.0f),
        vec3(0.0f, 0.0f, -1.0f),
        vec3(0.0f, 0.0f, -1.0f)
    },
    {
        vec3(-1.0f, 1.0f, -1.0f),
        vec3(0.0f, 0.0f, -1.0f),
        vec3(0.0f, 0.0f, -1.0f)
    },
    {
        vec3(-1.0f, -1.0f, 1.0f),
        vec3(0.0f, 0.0f, -1.0f),
        vec3(0.0f, 0.0f, 1.0f)
    },
    {
        vec3(1.0f, -1.0f, 1.0f),
        vec3(0.0f, 0.0f, -1.0f),
        vec3(0.0f, 0.0f, 1.0f)
    },
    {
        vec3(1.0f, 1.0f, 1.0f),
        vec3(0.0f, 0.0f, -1.0f),
        vec3(0.0f, 0.0f, 1.0f)
    },
    {
        vec3(-1.0f, 1.0f, 1.0f),
        vec3(0.0f, 0.0f, -1.0f),
        vec3(0.0f, 0.0f, 1.0f)
    }
};

vector<GLuint> cube_indices = {
	0, 1, 3, 3, 1, 2,
	1, 5, 2, 2, 5, 6,
	5, 4, 6, 6, 4, 7,
	4, 0, 7, 7, 0, 3,
	3, 2, 7, 7, 2, 6,
	4, 5, 0, 0, 5, 1
};


double lastX;
double lastY;
const float sensitivity = 0.25f;
bool   firstMouse = true;
float  camera_Speed_Multiplier;
float yaw_, pitch_;


void Core::Game::OnInit(){

}

void input(){
	// Simple camera movement function
    Entity camera = game.renderSystem->cameraEntity;
    auto cameraTransform = game.GetComponent<Transform>(camera);
    auto cameraComponent = game.GetComponent<Camera>(camera);

	float cameraSpeed = 2.5f * game.delta_time * camera_Speed_Multiplier;
    if (game.keys[GLFW_KEY_W])
        cameraTransform->position += cameraSpeed * cameraComponent->front;
    if (game.keys[GLFW_KEY_S])
       cameraTransform->position  -= cameraSpeed * cameraComponent->front;
    if (game.keys[GLFW_KEY_A])
        cameraTransform->position  -= glm::normalize(glm::cross(cameraComponent->front, cameraComponent->up)) * cameraSpeed;
    if (game.keys[GLFW_KEY_D])
        cameraTransform->position  += glm::normalize(glm::cross(cameraComponent->front, cameraComponent->up)) * cameraSpeed;
    if (game.keys[GLFW_KEY_LEFT_SHIFT]){
    	camera_Speed_Multiplier = 3.0f;
    }else{
    	camera_Speed_Multiplier = 1.0f;
    }
    
}

void inputMouse(){
	// Simple mouse look function
    Entity camera = game.renderSystem->cameraEntity;
    auto cameraComponent = game.GetComponent<Camera>(camera);
    
	if(!game.mouseLock){return;}
	if (firstMouse) // initially set to true
	{
	    lastX = game.mouseXPos;
	    lastY = game.mouseYPos;
	    firstMouse = false;
	}

	float xoffset = game.mouseXPos - lastX;
	float yoffset = lastY - game.mouseYPos;
	lastX = game.mouseXPos;
	lastY = game.mouseYPos;

	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw_   += xoffset;
	pitch_ += yoffset;  

	if(pitch_ > 89.0f)
	  pitch_ =  89.0f;
	if(pitch_ < -89.0f)
	  pitch_ = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
	direction.y = sin(glm::radians(pitch_));
	direction.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
	cameraComponent->front = glm::normalize(direction);
    
}

void Core::Game::OnUpdate(){
	input();
	inputMouse();
}

void Core::Game::OnRender(){

}

void Core::Game::OnLast(){

}

int main(){
	// Enables the game's debug mode
	game.SetDebugMode(true);

	// Sets the up and front camera coordinates
    /*
	camera.front = glm::vec3(0.0f,0.0f,-1.0f);
	camera.up = glm::vec3(0.0f, 1.0f, 0.0f);
    */

    auto newEnt = game.CreateEntity("Hello Entity");

    Shader* cube_shader = game.resourceManager.LoadShader("a", "res/shaders/standard/standard.vs", "res/shaders/standard/standard.fs");

    game.AddComponent(newEnt, Transform{glm::vec3(0,0,0),glm::vec3(0),glm::vec3(1)});
    game.AddComponent(newEnt, MeshRenderer{glm::vec3(255), new Mesh(&cube_vertices, &cube_indices, MESH_3D_TEXTURED_NORMAL) , cube_shader});

    auto physEnt = game.CreateEntity("Cube");
    game.AddComponent(physEnt, Transform{ glm::vec3(0,0,0),glm::vec3(0),glm::vec3(1) });
    game.AddComponent(physEnt, MeshRenderer{ glm::vec3(120, 120, 0), new Mesh(&cube_vertices, &cube_indices, MESH_3D_TEXTURED_NORMAL) , cube_shader });
    game.AddComponent(physEnt, RigidBody{ glm::vec3(0), glm::vec3(0) } );

    // Runs the game
	game.Run();
	return 0;
}

