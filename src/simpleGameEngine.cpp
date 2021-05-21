#include <stdio.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "mesh.h"
#include "logger.h"
#include "simpleGameEngine.h"
#include "camera.h"
#include "scene.h"
#include "systemManager.h"
#include "components.h"

namespace Lynx {

bool Game::mouseLock;
int Game::polygonMode;
bool Game::keys[1024];
bool Game::debugMode;
float Game::pitch;
float Game::yaw;
float Game::lastX;
float Game::lastY;
double Game::mouseXPos, Game::mouseYPos;
bool Game::firstMouse;

Game::Game(unsigned int width, unsigned int height):
	logger("main.log", LOG_DEBUG, false),
    resourceManager(&logger)
{
    
	WINDOW_WIDTH = width;
	WINDOW_HEIGHT = height;

	initWindow();

}


Game::~Game(){
	OnLast();
	ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
	glfwTerminate();
}

void Game::SetDebugMode(bool mode){
	debugMode = mode;
}

void Game::initWindow(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
  
  	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, windowName, NULL, NULL);
	if (window == NULL)
	{
	    printf("Failed to create window\n");
	    glfwTerminate();
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Limit FPS
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);  
	glfwSetCursorPosCallback(window, MouseCallback);  
	glfwSetKeyCallback(window, KeyCallback);

    mouseLock = false;

	bool err = glewInit() != GLEW_OK;   

    if(err){
        printf("Failed to initialize GLEW ! \n");
    }

	glEnable(GL_DEPTH_TEST);

	IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    componentManager = std::make_unique<ComponentManager>();
    entityManager = std::make_unique<EntityManager>();
    systemManager = std::make_unique<SystemManager>();

    RegisterComponent<const char*>();
    RegisterComponent<Transform>();
    RegisterComponent<RigidBody>();

    OnInit();


}


/*
*
*   Entity Component System
* 
*/



Entity Game::CreateEntity() {
    return entityManager->CreateEntity();
}


Entity Game::CreateEntity(const char* name) {
    Entity newEnt = entityManager->CreateEntity();
    AddComponent(newEnt, name);
    return newEnt;
}


void Game::DestroyEntity(Entity entity) {
    entityManager->DestroyEntity(entity);
}


/*
*   End of ECS
*/

// Main Loop

void Game::Run(){
	while((!glfwWindowShouldClose(window))|running)
	{
        float current_FrameTime = glfwGetTime();
		delta_time = current_FrameTime - last_FrameTime;
		last_FrameTime = current_FrameTime;
        glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
		
		OnUpdate();
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if(debugMode){
			DebugWindow();
		}

		OnRender();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	    glfwSwapBuffers(window);  
	    glfwPollEvents();

    }
}

// Game Input

void Game::MouseCallback(GLFWwindow* window, double xpos, double ypos){
    mouseXPos = xpos;
    mouseYPos = ypos;
}

void Game::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	/*

    if (key ==  GLFW_KEY_I && action == GLFW_PRESS){
        if(polygonMode<1){polygonMode++;}else{polygonMode = 0;}
    	//glPolygonMode(GL_FRONT_AND_BACK, polygonModes[polygonMode]);

    }

    */

    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    	glfwSetWindowShouldClose(window, true);



    if(key >= 0 && key < 1024){
    	if(action == GLFW_PRESS)
    		keys[key] = true;
    	else if(action == GLFW_RELEASE)
    		keys[key] = false;

    }

    if(keys[GLFW_KEY_LEFT_SHIFT]&&keys[GLFW_KEY_LEFT_CONTROL]&&keys[GLFW_KEY_F1])
    	if(debugMode){debugMode = false;}else{debugMode = true;}
    

    if(keys[GLFW_KEY_LEFT_SHIFT]&&keys[GLFW_KEY_LEFT_CONTROL]&&keys[GLFW_KEY_F2]){
    	if(mouseLock){
    		mouseLock = false; 
    		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    	}else{
    		mouseLock = true; 
    		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    	}
    }


}

void Game::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    /*
    for ( const auto &cam : Scenes[activeScene]->Cameras){
        cam->resX = width;
        cam->resY = height;
    }
    */
}  

// Game Debug / Editor stuff

void Game::DebugWindow(){
    if (ImGui::BeginMainMenuBar())
    {
        if(ImGui::BeginMenu("File")){
            if (ImGui::MenuItem("New")) {}
            if (ImGui::MenuItem("Open", "Ctrl+O")) {}
            if (ImGui::MenuItem("Export scene", "Ctrl+LShift+E")) {}
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("Edit")){
            if (ImGui::MenuItem("Preferences")) {}
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("Window")){
            if (ImGui::MenuItem("Open inspector")) { if(!inspectorToggle){inspectorToggle=true;}else{inspectorToggle=false;} }
            if (ImGui::MenuItem("Debug overlay")) { if(!overlayToggle){overlayToggle=true;}else{overlayToggle=false;} }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    if(inspectorToggle){InspectorWindow();}
    if(overlayToggle){DebugOverlay();}

}


void Game::InspectorWindow(){
    ImGui::Begin("Hierarchy");
    //ImGui::Text("Current Scene : Scene #%d ( %s ) ", activeScene, Scenes[activeScene]->name);  
	//ImGui::Text("FPS : %d", (int)round(1/delta_time));
    

    for (int id = 0; entityManager->livingEntityCount; id++) {

        if (ImGui::Button(GetComponent<const char*>(id)))
        {
            selectedId = id;
        }
        
    }
  
    /*
    ImGui::Separator();
    ImGui::Text("Resources");

    for(const auto &shdr : resourceManager.ShaderMap){
        if (ImGui::Button(shdr.first))
        {
            selectedType = 4;
            selectedShader = shdr.second;
            selectedName = shdr.first;
        }
    }
    */
    ImGui::End();

    ImGui::Begin("Inspector");
    
    if (selectedId > 0) {
        auto signature = entityManager->GetSignature(selectedId);

        if (signature.test(componentManager->GetComponentType<Transform>())) {
            ImGui::Text("Position : ");
            ImGui::SameLine();
            ImGui::InputFloat3("##1", glm::value_ptr(GetComponent <Transform>(selectedId).position));

            ImGui::Text("Rotation : ");
            ImGui::SameLine();
            ImGui::InputFloat3("##2", glm::value_ptr(GetComponent <Transform>(selectedId).rotation));

            ImGui::Text("Scale : ");
            ImGui::SameLine();
            ImGui::InputFloat3("##3", glm::value_ptr(GetComponent <Transform>(selectedId).scale));
        }
    }

    /* OLD INSPECTOR
    if(selectedType == 1){
        ImGui::Text("Selected : %s",  selectedName);
        ImGui::Text("Type : 2D Sprite");

        ImGui::Text("XYZ : ");
        ImGui::SameLine();
        ImGui::InputFloat3("##1",glm::value_ptr(selectedSprite->pos));
        ImGui::Text("Colour : ");
        ImGui::SameLine();
        ImGui::SliderFloat3("##2", glm::value_ptr(selectedSprite->color), 0.0f, 1.0f);
    }else if(selectedType == 2){
        ImGui::Text("Selected : %s", selectedName);
        ImGui::Text("Type : Camera");
        ImGui::Text("XYZ : ");
        ImGui::SameLine();
        ImGui::InputFloat3("##1",glm::value_ptr(selectedCamera->pos));
        ImGui::Text("Front (ang) : ");
        ImGui::SameLine();
        ImGui::InputFloat3("##2",glm::value_ptr(selectedCamera->front));
    }else if(selectedType == 3){
        ImGui::Text("Selected : %s", selectedName);
        ImGui::Text("Type : Mesh 3D");
        ImGui::Text("XYZ : ");
        ImGui::SameLine();
        ImGui::InputFloat3("##1",glm::value_ptr(selectedObject->pos));
    }else if(selectedType == 5){
        ImGui::Text("Selected : %s", selectedName);
        ImGui::Text("Type : 3D Model");
        ImGui::Text("XYZ : ");
        ImGui::SameLine();
        ImGui::InputFloat3("##1",glm::value_ptr(selectedModel->pos));
        for(int count = 0; count < selectedModel->meshes.size(); count++){
            if (ImGui::CollapsingHeader("Child mesh")){
                ImGui::Text("XYZ : ");
                ImGui::SameLine();
                ImGui::InputFloat3("##1",glm::value_ptr(selectedModel->meshes[count]->pos));
            }
        }
    }else if(selectedType == 4){
        ImGui::Text("Selected : %s", selectedName);
        ImGui::Text("Type : Shader");
        int attrib_count, uniform_count;
        glGetProgramiv(selectedShader->getProgram(), GL_ACTIVE_ATTRIBUTES, &attrib_count);
        glGetProgramiv(selectedShader->getProgram(), GL_ACTIVE_UNIFORMS, &uniform_count);
        if(!selectedShader->success){attrib_count = 0; uniform_count = 0;}
        ImGui::Text("Shader program ID : %d", selectedShader->ID);
        ImGui::Text("Vertex Shader File : %s", selectedShader->vertexFilePath);
        ImGui::Text("Fragment Shader File : %s", selectedShader->fragmentFilePath);
        ImGui::Text("Active attributes ( %d )", attrib_count);
        ImGui::Text("Active uniforms ( %d )", uniform_count);

        if(!selectedShader->success){
            ImGui::TextColored(ImVec4(1.0f,0.0f,0.0f,1.0f), "Shader compilation failed : ");
            ImGui::TextColored(ImVec4(1.0f,0.0f,0.0f,1.0f), selectedShader->getError());
        }

        if(ImGui::Button("Reload Shader")){selectedShader->Reload();}


        if(selectedShader->success){
            if (ImGui::CollapsingHeader("Attributes"))
            {
                
                for (int i = 0; i < attrib_count; i++)
                {

                    int count;
                    int length;
                    GLsizei size;
                    GLenum type;
                    char name[512];
                    glGetActiveAttrib(selectedShader->getProgram(), (GLuint)i, 512, &length, &size, &type, name);
                    if (ImGui::TreeNode("%s##%d", name, i))
                    {
                        ImGui::Text("Name %s ", name);
                        ImGui::Text("Type %u ", type);
                        ImGui::TreePop();
                    }
                }
            }

            if (ImGui::CollapsingHeader("Uniforms"))
            {
                
                for (int i = 0; i < uniform_count; i++)
                {

                    int count;
                    int length;
                    GLsizei size;
                    GLenum type;
                    char name[512];
                    glGetActiveUniform(selectedShader->getProgram(), (GLuint)i, 512, &length, &size, &type, name);
                    if (ImGui::TreeNode("%s##%d", name, i))
                    {
                        ImGui::Text("Name %s ", name);
                        ImGui::Text("Type %u ", type);
                        ImGui::TreePop();
                    }
                }
            }
        }
    }
    */
    ImGui::End();
}

void Game::DebugOverlay(){
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    
    const float PAD = 10.0f;
    static int corner = 0;

    ImGui::SetNextWindowBgAlpha(0.35f);
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 work_pos = viewport->WorkPos;
    ImVec2 work_size = viewport->WorkSize;
    ImVec2 window_pos, window_pos_pivot;

    window_pos.x = (corner & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
    window_pos.y = (corner & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
    window_pos_pivot.x = (corner & 1) ? 1.0f : 0.0f;
    window_pos_pivot.y = (corner & 2) ? 1.0f : 0.0f;
    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
    window_flags |= ImGuiWindowFlags_NoMove;
    
    ImGui::Begin("Debug", NULL, window_flags);
    ImGui::Text("FPS: %f", floor(1/delta_time));
    ImGui::Text("Frametime : %f", delta_time*1000);

    ImGui::End();
    
}

}
