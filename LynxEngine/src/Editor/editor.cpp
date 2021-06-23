#include "editor.h"
#include "ECS/components.h"
#include "Core/simpleGameEngine.h"
#include "Core/logger.h"

extern Lynx::Core::Game game;

namespace Lynx {

		Editor::Editor()
		{
			node_flags = ImGuiTreeNodeFlags_Selected | ImGuiTreeNodeFlags_OpenOnDoubleClick;
		}

		void Editor::Init()
		{
			auto io = ImGui::GetIO();
			ImFontConfig config;
			io.Fonts->AddFontFromFileTTF("res/fonts/roboto.ttf",16.0f, &config);
			io.Fonts->Build();
			setupStyle();
		}

		void Editor::setupStyle()
		{
			ImGuiStyle& style = ImGui::GetStyle();
			style.Alpha = 1.0f;
			style.FrameRounding = 3.0f;
			style.Colors[ImGuiCol_Text]                  = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
			style.Colors[ImGuiCol_TextDisabled]          = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
			style.Colors[ImGuiCol_WindowBg]              = ImVec4(0.14f, 0.14f, 0.14f, 0.94f);
			style.Colors[ImGuiCol_PopupBg]               = ImVec4(0.10f, 0.10f, 0.10f, 0.94f);
			style.Colors[ImGuiCol_Border]                = ImVec4(1.00f, 1.00f, 1.00f, 0.39f);
			style.Colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.10f);
			style.Colors[ImGuiCol_FrameBg]               = ImVec4(0.00f, 0.00f, 0.00f, 0.94f);
			style.Colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.86f, 0.19f, 0.08f, 0.40f);
			style.Colors[ImGuiCol_FrameBgActive]         = ImVec4(0.86f, 0.19f, 0.08f, 0.67f);
			style.Colors[ImGuiCol_TitleBg]               = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
			style.Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
			style.Colors[ImGuiCol_TitleBgActive]         = ImVec4(0.42f, 0.42f, 0.42f, 1.00f);
			style.Colors[ImGuiCol_MenuBarBg]             = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
			style.Colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.18f, 0.18f, 0.18f, 0.53f);
			style.Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
			style.Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
			style.Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
			style.Colors[ImGuiCol_CheckMark]             = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
			style.Colors[ImGuiCol_SliderGrab]            = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
			style.Colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
			style.Colors[ImGuiCol_Button]				 = ImVec4(0.96f, 0.29f, 0.08f, 0.40f);
        	style.Colors[ImGuiCol_ButtonHovered]  		 = ImVec4(0.96f, 0.29f, 0.08f, 1.00f);
        	style.Colors[ImGuiCol_ButtonActive] 		 = ImVec4(0.96f, 0.23f, 0.08f, 1.00f);
			style.Colors[ImGuiCol_Header] 				 = ImVec4(0.00f, 0.00f, 0.00f, 0.94f);
			style.Colors[ImGuiCol_HeaderHovered]		 = ImVec4(0.16f, 0.39f, 0.48f, 0.40f);
			style.Colors[ImGuiCol_HeaderActive]			 = ImVec4(0.16f, 0.39f, 0.28f, 0.67f);
			style.Colors[ImGuiCol_ResizeGrip]            = ImVec4(0.00f, 0.00f, 0.00f, 0.50f);
			style.Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.86f, 0.59f, 0.08f, 0.67f);
			style.Colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.86f, 0.59f, 0.08f, 0.95f);
			style.Colors[ImGuiCol_PlotLines]             = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
			style.Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(0.10f, 0.43f, 0.35f, 1.00f);
			style.Colors[ImGuiCol_PlotHistogram]         = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
			style.Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
			style.Colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
		}
	
		void Editor::Draw() 
		{
			if (ImGui::BeginMainMenuBar())  {
				if(ImGui::BeginMenu("File")){   
					if (ImGui::MenuItem("New")) {}  
					if (ImGui::MenuItem("Open", "Ctrl+O")) {}
					if (ImGui::MenuItem("Export scene", "Ctrl+LShift+E")) {}
					ImGui::EndMenu();
				}
				if(ImGui::BeginMenu("Edit")){   
					if (ImGui::MenuItem("Preferences")) {}
					ImGui::EndMenu();
				}
				if(ImGui::BeginMenu("Window")){ 
					if (ImGui::MenuItem("Open Console")) { consoleActive = !consoleActive; }
					if (ImGui::MenuItem("Debug overlay")) { debugOverlayActive = !debugOverlayActive; }
					ImGui::EndMenu();
				}
				ImGui::EndMainMenuBar();        
    		}
			drawHierarchy();
			drawInspector();

			if(debugOverlayActive)
				drawDebugOverlay();
			
			if(consoleActive)
				console_draw();
		}

		void Editor::drawHierarchy()
		{
			ImGui::Begin("Hierarchy");

			for(int id = 0; id < game.entityManager->livingEntityCount; id++) {
				// Check if node is a child node
				if(game.GetComponent<Generic>(id)->isChild)
					continue;
				

				drawHierarchyItem(id);
				
			}	

			ImGui::End();
		}

		void Editor::drawHierarchyItem(int id)
		{
			ImGui::PushID(id);

			bool open = ImGui::TreeNodeEx(game.componentManager->GetComponent<Generic>(id)->name, node_flags);

			if(ImGui::IsItemClicked()){
				selectedId = id;
			}

			if(open){
				for ( int c = 0; c < game.entityManager->livingEntityCount; c++){
					if(!game.GetComponent<Generic>(c)->isChild)
						continue;
					if(game.GetComponent<Parent>(c)->parentEntity == id)
						drawHierarchyItem(id);
					
				}
				ImGui::TreePop();
			}

			ImGui::PopID();
		}

		void Editor::drawInspector()
		{

			ImGui::Begin("Inspector");

			auto signature = game.entityManager->GetSignature(selectedId);
			int id = 0;


			
			if (signature.test(game.componentManager->GetComponentType<Transform>())) {
				if (ImGui::CollapsingHeader("Transform")) {
					ImGui::Text("Position : ");
					ImGui::SameLine();
					ImGui::InputFloat3("##1", glm::value_ptr(game.GetComponent <Transform>(selectedId)->position));

					ImGui::Text("Rotation : ");
					ImGui::SameLine();
					ImGui::InputFloat3("##2", glm::value_ptr(game.GetComponent <Transform>(selectedId)->rotation));

					ImGui::Text("Scale : ");
					ImGui::SameLine();
					ImGui::InputFloat3("##3", glm::value_ptr(game.GetComponent <Transform>(selectedId)->scale));
				}
			}

			if (signature.test(game.componentManager->GetComponentType<MeshRenderer>())) {

				if (ImGui::CollapsingHeader("Mesh Renderer")) {
					auto comp = game.GetComponent <MeshRenderer>(selectedId);
					ImGui::Text("Color : ");
					ImGui::SameLine();
					ImGui::InputFloat3("##4", glm::value_ptr(comp->color));
				}

			}

			if (signature.test(game.componentManager->GetComponentType<Camera>())) {
				if (ImGui::CollapsingHeader("Camera")) {
					auto comp = game.GetComponent <Camera>(selectedId);
					ImGui::Text("FOV : ");
					ImGui::SameLine();
					ImGui::InputFloat("##5", &comp->FOV);

					ImGui::Text("Resolution : ");
					ImGui::SameLine();
					ImGui::InputFloat2("##6", glm::value_ptr(comp->res));

					ImGui::Text("Camera type : ");
					ImGui::SameLine();
					switch(comp->type){
						case CAMERA_PERSPECTIVE:
							ImGui::Text("Perspective");
							break;
						case CAMERA_ORTHOGRAPHIC:
							ImGui::Text("Orthographic");
							break;
					};

					ImGui::Checkbox("Is main? ", &comp->isMain);

				}
			}

			ImGui::End();
		}

		void Editor::drawDebugOverlay()
		{
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
			
			const float PAD = 10.0f;
			static int corner = 0;

			ImGui::SetNextWindowBgAlpha(0.35f);
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImVec2 work_pos = viewport->WorkPos;
			ImVec2 work_size = viewport->WorkSize;
			ImVec2 window_pos, window_pos_pivot;

			window_pos.x = (corner & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
			window_pos.y = (corner & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
			window_pos_pivot.x = (corner & 1) ? 1.0f : 0.0f;
			window_pos_pivot.y = (corner & 2) ? 1.0f : 0.0f;
			ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
			window_flags |= ImGuiWindowFlags_NoMove;
			
			ImGui::Begin("Debug", NULL, window_flags);
			ImGui::Text("FPS: %f", floor(1/game.delta_time));
			ImGui::Text("Frametime : %f", game.delta_time*1000);
			ImGui::End();
		}
}
