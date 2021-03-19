#include <stdio.h>
#include "simpleGameEngine.h"
#include <GLFW/glfw3.h>

Game game(1280,720);

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

    if(key >= 0 && key < 1024){
    	if(action == GLFW_PRESS)
    		game.keys[key] = true;
    	else if(action == GLFW_RELEASE)
    		game.keys[key] = false;

    }
}

void Game::OnInit(){

}

void Game::OnUpdate(){
	if(game.keys[GLFW_KEY_ESCAPE]){
		glfwSetWindowShouldClose(game.window, true);
	}
}

void Game::OnRender(){

}

void Game::OnLast(){

}

int main(){
	glfwSetKeyCallback(game.window, KeyCallback);
	game.Run();
	return 0;
}