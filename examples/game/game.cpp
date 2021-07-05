#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <iostream>
#include <vector>
#include <glm/glm.hpp>

#include "Core/simpleGameEngine.h"
#include "Core/windowManager.h"
#include "Core/eventManager.h"

#include "Graphics/texture.h"
#include "Graphics/model.h"

#include "ECS/components.h"

using namespace Lynx;

// Initialize global variables

WindowManager gWindowManager;
EventManager gEventManager;
ResourceManager gResourceManager;
Game game;


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

void Init()
{

}

void Update()
{

}

int main()
{

	// Initialize window in windowed mode
	gWindowManager.Init("Example Game", 1270, 720, false);

	// Enables the game's debug mode
	game.SetDebugMode(true);
	game.Init();
	log_set_level(LOG_DEBUG);
	
	Shader* cube_shader = gResourceManager.LoadShader("Lighting Shader", "res/shaders/standard/lighting.vs", "res/shaders/standard/lighting.fs");
	Shader* light_shader = gResourceManager.LoadShader("Cube Shader", "res/shaders/standard/standard.vs", "res/shaders/standard/standard.fs");
	Texture* texture = gResourceManager.LoadTexture("prototype", "res/images/container.jpg");
	Mesh* cube_mesh = gResourceManager.LoadMesh("Cube", &cube_vertices, &cube_indices, MESH_3D);

	log_info("Loading model");
	auto ModelEntity = ModelLoader::loadModel("res/models/cube.fbx", cube_shader);

	auto lightEnt = game.CreateEntity("Point Light");
	game.AddComponent(lightEnt, Transform{ glm::vec3(0, 5, 15), glm::vec3(0), glm::vec3(0.25) });
	game.AddComponent(lightEnt, MeshRenderer{ glm::vec3(255), cube_mesh, light_shader});
	game.AddComponent(lightEnt, PointLight{ glm::vec3(255), glm::vec3(255), glm::vec3(255), 1.0f, 0.09f, 0.032f });
	
	/*
	auto physEnt = game.CreateEntity("Cube");
    game.AddComponent(physEnt, Transform{ glm::vec3(0,0,0),glm::vec3(0),glm::vec3(1) });
    game.AddComponent(physEnt, MeshRenderer{ glm::vec3(120, 120, 0), new Mesh(&cube_vertices, &cube_indices, MESH_3D_TEXTURED_NORMAL) , cube_shader });
   	game.AddComponent(physEnt, RigidBody{ glm::vec3(0), glm::vec3(0) } );
	*/



	// Runs the game
	game.Run();
	return 0;
}