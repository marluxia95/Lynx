#include <stdio.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "application.h"

#include "window_manager.h"
#include "input.h"

#include "event_manager.h"
#include "Events/event.h"

#include "module.h"

#include "logger.h"

#ifdef __linux__
#include <unistd.h>
#endif

#include <GLFW/glfw3.h>

namespace Lynx {

    Application* Application::s_applicationInstance = nullptr;

    Application::Application() 
    {
        log_debug("Initializing subsystems");


        thread_id = std::this_thread::get_id();

#ifdef LYNX_MULTITHREAD
        log_warn("Multithreading is enabled ! Keep in mind that this is still in progress and the application might not work as intended !");
        m_threadPool = std::make_shared<ThreadPool>(3);
#endif

        s_applicationInstance = this;
    }

    Application::~Application()
    {
        ModuleManager::UnloadAllModules();
#ifdef LYNX_MULTITHREAD
        m_threadPool->Wait();
#endif
    }

    /**
     * @brief Initializes the application instance
     * 
     * @param flags Not used
     */
    void Application::Initialise(int flags)
    {
        log_debug("Sending event init");
        EventManager::SendEvent(InitEvent());

        log_debug("Initializing systems");

        log_debug("Successfully initialized application");
    }

    /**
     * @brief Calculates the frametime
     * 
     */
    void Application::CalculateFrameTime()
    {
        float current_FrameTime = glfwGetTime();
        delta_time = current_FrameTime - last_FrameTime;
        last_FrameTime = current_FrameTime;
    }

    /**
     * @brief Runs the main loop
     * 
     */
    void Application::Run()
    {
        do
        {
            CalculateFrameTime();
#ifdef LYNX_MULTITHREAD
            EventManager::UpdateListeners();
#endif
            if(applicationState == STATE_ACTIVE) {
                EventManager::SendEvent(UpdateTickEvent());

                // Update logic
                EventManager::SendEvent(RenderEvent());
            }

        } while(applicationState != STATE_CLOSED);
        EventManager::SendEvent(LastTickEvent());
    }

}
