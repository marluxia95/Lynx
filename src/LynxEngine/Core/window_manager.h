#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include "lynx_common.h"

namespace Lynx {

    class LYNXENGINE_API WindowManager {
		public:
			void Init(const char* title = "Lynx Engine", unsigned int width = 1270, unsigned int height = 720, bool fullScreen = false);
			
            void Update();

			void SetTitle(const char* title);

            void Destroy();

			bool ShouldClose();

			uint GetWidth() { return window_width; }

			uint GetHeight() { return window_height; }
		private:
			unsigned int window_width = 1280;
			unsigned int window_height = 720;
	};


}

#endif