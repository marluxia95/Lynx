#include <stdio.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include "simpleGameEngine.h"
#include "texture.h"
#include "camera.h"
#include "model.h"
#include "scene.h"
#include "light.h"
#include "components.h"

using namespace Lynx;

// Initialize global variables
Game game(1280,720);
Camera camera(CAMERA_PERSPECTIVE,1280, 720);

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


void Game::OnInit(){

}

void input(){
	// Simple camera movement function
	float cameraSpeed = 2.5f * game.delta_time * camera_Speed_Multiplier;
    if (game.keys[GLFW_KEY_W])
        camera.pos += cameraSpeed * camera.front;
    if (game.keys[GLFW_KEY_S])
        camera.pos -= cameraSpeed * camera.front;
    if (game.keys[GLFW_KEY_A])
        camera.pos -= glm::normalize(glm::cross(camera.front, camera.up)) * cameraSpeed;
    if (game.keys[GLFW_KEY_D])
        camera.pos += glm::normalize(glm::cross(camera.front, camera.up)) * cameraSpeed;
    if (game.keys[GLFW_KEY_LEFT_SHIFT]){
    	camera_Speed_Multiplier = 3.0f;
    }else{
    	camera_Speed_Multiplier = 1.0f;
    }
}

void inputMouse(){
	// Simple mouse look function
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
	camera.front = glm::normalize(direction);
}

void Game::OnUpdate(){
	input();
	inputMouse();
}

void Game::OnRender(){

}

void Game::OnLast(){

}

int main(){
	// Enables the game's debug mode
	game.SetDebugMode(true);

	// Sets the up and front camera coordinates
	camera.front = glm::vec3(0.0f,0.0f,-1.0f);
	camera.up = glm::vec3(0.0f, 1.0f, 0.0f);
	
    printf("Creating entity\n");
    auto newEnt = game.CreateEntity("Hello Entity");
    printf("Adding component\n");
    Transform entPos = { glm::vec3(0,0,0), glm::quat(0,0,0,0), glm::vec3(0,0,0) };
    game.AddComponent(newEnt, Transform{glm::vec3(0,0,0),glm::quat(0,0,0,0),glm::vec3(0)});

	// Runs the game
	game.Run();
	return 0;
}

