/**
 * @file application.h
 * @author marlxx (marlxx@protonmail.com)
 * @brief This contains all the main application logic, loops, etc. 
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef APPLICATION_H
#define APPLICATION_H

#include <stdio.h>
#include <memory>

#include "scene.h"
#include "window_manager.h"
#include "resource_manager.h"
#include "threadpool.h"

namespace Lynx {

	enum State{
		STATE_ACTIVE,
        STATE_PAUSED,
        STATE_CLOSED
	};

    enum {
        APPLICATION_FULLSCREEN,
        APPLICATION_BORDERLESS = 1 << 1,
        APPLICATION_VSYNC = 1 << 2
    };

    class ResourceManager;

	class LYNXENGINE_API Application {
    public:
        Application();
		~Application();

        virtual void Initialise(int flags);

		virtual void Run();
        
        float GetDeltaTime() { return delta_time; }

        void SetApplicationState(State state) { applicationState = state; }

        std::shared_ptr<ResourceManager> GetResourceManager() { return m_resourceManager; }

        std::thread::id GetThread() { return thread_id; };

        static Application* GetSingleton() { return s_applicationInstance; }
    protected:
        void CalculateFrameTime();

    public:
        friend class ResourceManager;
    private:
        static Application* s_applicationInstance;
    protected:
        float delta_time, last_FrameTime = 0.0f;
		State applicationState;
        std::thread::id thread_id;
        std::unique_ptr<ThreadPool> m_threadPool;	
        std::unique_ptr<WindowManager> m_windowManager;
        std::shared_ptr<ResourceManager> m_resourceManager;
        
	};

}


#endif