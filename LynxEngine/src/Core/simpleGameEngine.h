#ifndef SIMPLEGAMEENGINE_H
#define SIMPLEGAMEENGINE_H

#include <stdio.h>
#include <iostream>
#include <vector>
#include <map>
#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <memory>

#include "resourceManager.h"
#include "windowManager.h"

#include "Graphics/renderer.h"
#include "Graphics/model.h"
#include "Graphics/mesh.h"

#include "ECS/systemManager.h"

#include "Events/event.h"

#include "windowManager.h"

#include "scene.h"

#include "Editor/editor.h"

#include "Systems/parentingSystem.h"
#include "Systems/lightingSystem.h"
#include "Systems/physicsSystem.h"
#include "Systems/renderSystem.h"
#include "Systems/cameraSystem.h"

namespace Lynx {

	enum GameState{
		GAME_PAUSE,
		GAME_ACTIVE
	};
	
	class Game {
	public:
		~Game();

		void Init();
		void Run();


        template<typename T>
        std::shared_ptr<T> RegisterSystem(){
            return m_systemManager->RegisterSystem<T>();
        }

        template<typename T>
        void SetSystemSignature(Signature signature){
            m_systemManager->SetSignature<T>(signature);
        }


		float delta_time, last_FrameTime;

		friend class Lynx::Editor;
	private:
		bool running;
		std::unique_ptr<Scene> m_scene;
		std::unique_ptr<ECS::SystemManager> m_systemManager;
		std::unique_ptr<WindowManager> m_windowManager;

	};

}
#endif
