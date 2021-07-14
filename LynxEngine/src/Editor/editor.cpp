#include "editor.h"
#include "ECS/components.h"
#include "Core/simpleGameEngine.h"
#include "Core/logger.h"

extern Lynx::Game game;

namespace Lynx {

		Editor::Editor()
		{
			node_flags = ImGuiTreeNodeFlags_Selected | ImGuiTreeNodeFlags_OpenOnDoubleClick;
		}

		template<typename T>
		void Editor::RegisterInspectorFunc(inspectorFunc func)
		{
			const char* name = typeid(T).name();
			inspectorFunctions[name] = func;
		}

		template<typename T>
		void Editor::runInspectorFunc(Entity ent)
		{
			const char* name = typeid(T).name();
			inspectorFunctions[name](ent);
		}

		void Editor::Init()
		{

			
			RegisterInspectorFunc<Transform>([](Entity ent){
				auto transform = game.GetComponent <Transform>(ent);
				if (ImGui::CollapsingHeader("Transform")) {
					ImGui::Text("Position : ");
					ImGui::SameLine();
					ImGui::InputFloat3("##1", glm::value_ptr(transform->position));

					ImGui::Text("Rotation : ");
					ImGui::SameLine();
					ImGui::InputFloat3("##2", glm::value_ptr(transform->rotation));

					ImGui::Text("Scale : ");
					ImGui::SameLine();
					ImGui::InputFloat3("##3", glm::value_ptr(transform->scale));
				}
			});

			RegisterInspectorFunc<MeshRenderer>([](Entity ent){
				if (ImGui::CollapsingHeader("Mesh Renderer")) {
					auto mrenderer = game.GetComponent <MeshRenderer>(ent);
					ImGui::Text("Ambient color : ");
					ImGui::SameLine();
					ImGui::InputFloat3("##4", glm::value_ptr(mrenderer->ambient));
					ImGui::Text("Diffuse color : ");
					ImGui::SameLine();
					ImGui::InputFloat3("##5", glm::value_ptr(mrenderer->diffuse));
					ImGui::Text("Specular color : ");
					ImGui::SameLine();
					ImGui::InputFloat3("##6", glm::value_ptr(mrenderer->specular));
				}
			});

			RegisterInspectorFunc<Camera>([](Entity ent){
				if (ImGui::CollapsingHeader("Camera")) {
					auto comp = game.GetComponent <Camera>(ent);
					ImGui::Text("FOV : ");
					ImGui::SameLine();
					ImGui::InputFloat("##7", &comp->FOV);

					ImGui::Text("Resolution : ");
					ImGui::SameLine();
					ImGui::InputFloat2("##8", glm::value_ptr(comp->res));

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
			});

			RegisterInspectorFunc<Shader>([](uint64_t id){
				
			});
		}

		void Editor::setupStyle()
		{

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
					if (ImGui::MenuItem("Console")) { consoleActive = !consoleActive; }
					if (ImGui::MenuItem("Debug overlay")) { debugOverlayActive = !debugOverlayActive; }
					ImGui::EndMenu();
				}
				ImGui::EndMainMenuBar();        
    		}
			//drawHierarchy();
			//drawInspector();

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

			if(selectedId){
				if(signature.test(game.componentManager->GetComponentType<Transform>()))
					runInspectorFunc<Transform>(selectedId);

				if(signature.test(game.componentManager->GetComponentType<MeshRenderer>()))
					runInspectorFunc<MeshRenderer>(selectedId);

				if(signature.test(game.componentManager->GetComponentType<Camera>()))
					runInspectorFunc<Camera>(selectedId);
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
