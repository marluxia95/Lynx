#include <GL/glew.h> 
#include "windowManager.h"
#include "logger.h"
#include "simpleGameEngine.h"
#include "eventManager.h"
#include "Events/event.h"
#include "Events/windowEvent.h"
#include "Events/keyEvent.h"
#include "Events/mouseEvent.h"

extern Lynx::EventManager gEventManager;
extern Lynx::Game game;

namespace Lynx {

    // Creates a window instance
    void WindowManager::Init(const char* title, unsigned int width, unsigned int height, bool fullScreen)
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

        auto const monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

        
        if(!width|!height) {
            // Get monitor's resolution

        }
        
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

        glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height){
            gEventManager.SendEvent(new WindowResizeEvent(width, height));
        });  

        glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods){
            if(action == GLFW_PRESS)
                gEventManager.SendEvent(new KeyPressedEvent(key));
        });

        glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos){
            gEventManager.SendEvent(new MouseCallbackEvent(xpos, ypos));
        });

        glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods){
            if(action == GLFW_PRESS)
                gEventManager.SendEvent(new MouseButtonEvent(button));
        });

    }

    void WindowManager::Update()
    {
        glfwSwapBuffers(window);
    }

    void WindowManager::Destroy()
    {
        glfwTerminate();
    }

}
