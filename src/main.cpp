#include <stdio.h>
#include <glm/glm.hpp>
#include "simpleGameEngine.h"
#include "sprite.h"
#include "camera.h"
#include "scene.h"

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

	Camera camera(CAMERA_ORTHOGRAPHIC, 1280, 720);

	unsigned int scene = game.CreateScene("test");

	printf("ID of created scene : %d\n", scene);

	game.SetActiveScene(scene);

	Scene* scn = game.GetActiveScene();
	scn->AddCamera("penis", &camera);

	Texture* tex = game.resourceManager.CreateTexture("Test", "res/images/box.jpg");

	Sprite* example = new Sprite();
	example->texture = game.resourceManager.GetTexture("Test");
	scn->AddSprite("Sprite1", example);




	game.Run();
	return 0;
}

