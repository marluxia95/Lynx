#include <stdio.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include "simpleGameEngine.h"
#include "sprite.h"
#include "camera.h"
#include "scene.h"
#include "mesh.h"
#include "utils.h"

using namespace Lynx;

Game game(1280,720);


void Game::OnInit(){

}

void Game::OnUpdate(){

}

void Game::OnRender(){

}

void Game::OnLast(){
	printf("Cya\n");
}

int main(){



	game.SetDebugMode(true);

	Camera camera(CAMERA_PERSPECTIVE, 1280, 720);
	camera.front = glm::vec3(0.0f,0.0f,-1.0f);
	camera.up = glm::vec3(0.0f, 1.0f, 0.0f);

	unsigned int scene = game.CreateScene("test");
	game.SetActiveScene(scene);

	Scene* scn = game.GetActiveScene();
	scn->AddCamera("Camera 1", &camera);

	Shader* mesh_shader = new Shader("res/shaders/standard/textured_box.vs", "res/shaders/standard/textured_box.fs");

	Mesh3D* mesh = new Mesh3D(&cube_vertices, &cube_indices, mesh_shader, MESH_3D);
	scn->Add3DObject("cube",mesh);

	game.Run();
	return 0;
}

