#ifndef CAMERASYSTEM_H
#define CAMERASYSTEM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Graphics/camera.h"

#include "Core/ECS/system.h"

using namespace glm;

namespace Lynx {


	class CameraSystem : public ECS::System {
		public:
			void Init();
			void Update();
	};

}

#endif 
