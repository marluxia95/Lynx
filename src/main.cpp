#include <stdio.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include "simpleGameEngine.h"
#include "sprite.h"
#include "camera.h"
#include "scene.h"
#include "mesh.h"
#include "model.h"
#include "utils.h"

using namespace Lynx;

// Initialize global variables
Game game(1280,720);
Camera camera(1280, 720);


double lastX;
double lastY;
const float sensitivity = 0.25f;
bool   firstMouse = true;
float  camera_Speed_Multiplier;
float yaw_, pitch_;
void Game::OnInit(){

}

void input(){
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
	printf("Cya\n");
}

int main(){



	game.SetDebugMode(true);
	camera.front = glm::vec3(0.0f,0.0f,-1.0f);
	camera.up = glm::vec3(0.0f, 1.0f, 0.0f);

	game.resourceManager.CreateShader("mesh_shader", "res/shaders/standard/standard.vs", "res/shaders/standard/standard.fs");
	//Shader* shader = new Shader("res/shaders/standard/standard.vs", "res/shaders/standard/standard.fs");
	Model model1;
	model1.load("res/models/monkey.obj");
	Mesh3D* mesh = new Mesh3D(&cube_vertices, &cube_indices, game.resourceManager.GetShader("mesh_shader") , MESH_3D);
	Mesh3D* mesh2 = new Mesh3D(&model1.vertices, &model1.vert_indices, game.resourceManager.GetShader("mesh_shader") , MESH_3D);

	unsigned int scene = game.CreateScene("test");
	game.SetActiveScene(scene);

	Scene* scn = game.GetActiveScene();
	scn->AddCamera("Camera 1", &camera);
	scn->Add3DObject("cube",mesh);
	scn->Add3DObject("monke",mesh2);


	game.Run();
	return 0;
}

