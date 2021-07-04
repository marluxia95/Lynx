#include <GL/glew.h> 
#include "windowManager.h"
#include "logger.h"
#include "simpleGameEngine.h"

namespace Lynx {

    extern Game game;

    // Creates a window instance
    void WindowManager::Init(const char* title = "SimpleGameEngine", unsigned int width = 1270, unsigned int height = 720, bool fullScreen = false)
    {
        int success = glfwInit();
        if(success == GLFW_FALSE){
            log_fatal("Unable to initialize GLFW!");
            exit(0);
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

        const GLFWmonitor monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
        
        if(fullScreen){
            window = glfwCreateWindow(width, height, title, monitor, NULL);
        }

        window = glfwCreateWindow(width, height, title, NULL, NULL);

        if (window == NULL)
        {
            log_fatal("Failed to create window");
            glfwTerminate();
            exit(1);
        }

        glfwSwapInterval(1); 
        glfwMakeContextCurrent(window);

    }

    void WindowManager::Update()
    {
        glfwSwapBuffers(window);
    }

    void WindowManager::Destroy()
    {
        glfwTerminate();
    }

    GLFWwindow* WindowManager::Getwindow()
    {
        return window;
    }

}
