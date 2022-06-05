#include <GL/glew.h>
#include "windowManager.h"
#include "logger.h"
#include "application.h"
#include "eventManager.h"
#include "Events/event.h"
#include "Events/windowEvent.h"
#include "Events/keyEvent.h"
#include "Events/mouseEvent.h"

namespace Lynx {

    // Creates a window instance
    void WindowManager::Init(const char* title, unsigned int width, unsigned int height, bool fullScreen)
    {
        glfwInit();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        log_debug("Creating window object");
        
        if(fullScreen){
            window = glfwCreateWindow(width, height, title, glfwGetPrimaryMonitor(), NULL);
        }

        window = glfwCreateWindow(width, height, title, NULL, NULL);

        if (window == NULL)
        {
            log_fatal("Failed to create window");
            glfwTerminate();
            exit(1);
        }

        glfwMakeContextCurrent(window);
        glfwSwapInterval(1); 
        
        glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height){
            glViewport(0,0, width, height);
            EventManager::SendEvent(WindowResizeEvent(width, height));
        });  

        glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos){
            EventManager::SendEvent(MouseCallbackEvent(xpos, ypos));
        });

        glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods){
            EventManager::SendEvent(KeyPressedEvent(key, action));
        });

        glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods){
            EventManager::SendEvent(MouseButtonEvent(button, action));
        });

        log_debug("Initialized GLFW ( Ver. %s )", glfwGetVersionString());
        log_debug("%s", window != NULL ? "Window is created" : "Window is not created");
    }

    void WindowManager::SetTitle(const char* title)
    {
        glfwSetWindowTitle(window, title);
    }

    void WindowManager::Update()
    {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    void WindowManager::Destroy()
    {
        log_debug("Terminating %d", window == NULL);
        glfwTerminate();
    }

}
