#ifndef EDITOR_H
#define EDITOR_H
#include <stdio.h>
#include <unordered_map>
#include "imgui.h"
#include "ECS/entity.h"

namespace Lynx {

	using inspectorFunc = void(*)(Entity);

	class Editor {
		
		public:
			Editor();
			void Init();
			void Draw();
			
			template<typename T>
			void RegisterInspectorFunc(inspectorFunc func);
		private:
			void setupStyle();
			void drawHierarchy();
			void drawInspector();
			void drawDebugOverlay();

			void drawHierarchyItem(int id);

			template<typename T>
			void runInspectorFunc(Entity ent);

			ImGuiTreeNodeFlags node_flags;
			unsigned int selectedId;		
			int selectedType = 0;			
			bool debugOverlayActive = false;
			bool consoleActive = false;

			std::unordered_map<const char*, inspectorFunc> inspectorFunctions;
	};
}

#endif
