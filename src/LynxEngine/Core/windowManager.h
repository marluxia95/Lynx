#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "lynx_common.h"

namespace Lynx {

    class LYNXENGINE_API WindowManager {
		public:
			void Init(const char* title = "Lynx Engine", unsigned int width = 1270, unsigned int height = 720, bool fullScreen = false);
			
            void Update();

			void SetTitle(const char* title);
			
            void Destroy();

			unsigned int window_width = 1280;
			unsigned int window_height = 720;
			GLFWwindow* window;
	};


}

#endif