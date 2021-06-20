#ifndef EDITOR_H
#define EDITOR_H
#include "imgui.h"

namespace Lynx {

	class Editor {
		
		public:
			Editor();
			void Init();
			void Draw();
			
			
		private:
			void setupStyle();
			void drawHierarchy();
			void drawInspector();
			void drawDebugOverlay();

			void drawHierarchyItem(int id);

			ImGuiTreeNodeFlags node_flags;
			unsigned int selectedId;					
			bool debugOverlayActive = false;

	};
}

#endif
