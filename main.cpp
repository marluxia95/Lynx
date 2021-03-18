#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <math.h>

#include "shader.h"
#include "texture.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void gui_createWindow();


const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

float delta_time = 0.0f;
float last_FrameTime = 0.0f;
float camera_Speed_Multiplier = 1.0f;

glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
glm::vec3 cameraDirection;
float lastX = 400, lastY = 300;
float pitch, yaw;
bool firstMouse = true;

GLint polygonModes[] = {
	GL_FILL,
	GL_LINE
};



int polygonMode;
int textures;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
  
  	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
	    printf("Failed to create window\n");
	    glfwTerminate();
	    return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  
	glfwSetCursorPosCallback(window, mouse_callback);  
	glfwSetKeyCallback(window, key_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
	    printf("Failed to load GLAD\n");
	    return -1;
	}    

	glEnable(GL_DEPTH_TEST);

	//glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	// Texture loading

	Texture tex1("images/box.jpg", &textures, GL_NEAREST, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	// Create shader
	

	float vertices[] = {
	    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	glm::vec3 cubePositions[] = {
	    glm::vec3( 0.0f,  0.0f,  0.0f),
	    glm::vec3(1.2f, 1.0f, 2.0f),
	    glm::vec3(1.2f, 5.0f, 5.0f)
	};

	unsigned int VBO, VAO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1,	&EBO);

	glBindVertexArray(VAO);

	// Copies triangle's vertices into the vertex buffer's memory 
	glBindBuffer(GL_ARRAY_BUFFER, VBO);  
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	Shader shader1("shaders/light_box.vs", "shaders/light_box.fs");
    shader1.setVec3("objectColor", 1.0f, 0.5f, 0.5f);
    shader1.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

    Shader shader2("shaders/lamp_cube.vs", "shaders/lamp_cube.fs");

    Shader shader3("shaders/lamp_cube.shader");

	while(!glfwWindowShouldClose(window))
	{
		// Get current frametime and calculate deltaTime
		float current_FrameTime = glfwGetTime();
		delta_time = current_FrameTime - last_FrameTime;
		last_FrameTime = current_FrameTime;
		printf("FPS : %f\n", 1/delta_time);
		glfwPollEvents();
		// input
		
	    processInput(window);

	    

		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);;



		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		projection = glm::perspective(glm::radians(60.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		

		glUniformMatrix4fv(glGetUniformLocation(shader1.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(shader1.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(VAO);
		for(unsigned int i = 0;i<(sizeof(cubePositions)/sizeof(glm::vec3));i++){
			if(i%2){shader2.use();}else{shader1.use();}
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model,cubePositions[i]);
			glUniformMatrix4fv(glGetUniformLocation(shader1.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

	    glfwSwapBuffers(window);  
	    glfwPollEvents();
	    
	}
    
	glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    //glDeleteBuffers(1, &EBO);
    shader1.destroy();
    glfwTerminate();
	return 0;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){

	if (firstMouse) // initially set to true
	{
	    lastX = xpos;
	    lastY = ypos;
	    firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	const float sensitivity = 0.25f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	

	yaw   += xoffset;
	pitch += yoffset;  

	if(pitch > 89.0f)
	  pitch =  89.0f;
	if(pitch < -89.0f)
	  pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
}

void processInput(GLFWwindow *window)
{
	float cameraSpeed = 2.5f * delta_time * camera_Speed_Multiplier;
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
    	camera_Speed_Multiplier = 3.0f;
    }else{
    	camera_Speed_Multiplier = 1.0f;
    }

    	
    	
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key ==  GLFW_KEY_I && action == GLFW_PRESS){
        if(polygonMode<1){polygonMode++;}else{polygonMode = 0;}
    	glPolygonMode(GL_FRONT_AND_BACK, polygonModes[polygonMode]);

    }
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}  