#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include "lynx_common.h"

namespace Lynx {

    class LYNXENGINE_API WindowManager {
		public:
			void Init(const char* title = "Lynx Engine", unsigned int width = 1920, unsigned int height = 1080, bool fullScreen = false);
			
            void Update();

			void SetTitle(const char* title);

			void Destroy();

			bool ShouldClose();

			unsigned int GetWidth() { return window_width; }

			unsigned int GetHeight() { return window_height; }
		private:
			unsigned int window_width = 1280;
			unsigned int window_height = 720;
	};


}

#endif
