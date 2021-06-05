#ifndef CAMERASYSTEM_H
#define CAMERASYSTEM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "system.h"
#include "simpleGameEngine.h"

using namespace glm;

namespace Lynx {

	enum CameraType {
		CAMERA_ORTHOGRAPHIC,
		CAMERA_PERSPECTIVE
	};

	// Camera component : Contains all camera data so any other external component has access to it
	struct Camera {
		float FOV = 60;
		vec2 res;
		CameraType type;
		bool isMain;
		vec3 front;
		vec3 up;
		mat4 view;
		mat4 projection;
	};

	class CameraSystem : public ECS::System {
		public:
			void Init();
			void Update();

		private:
			mat4 GetProjection(Camera* camera);
			mat4 GetView(Entity entity, Camera* camera);
			Camera cameraComponent;
	};

}

#endif 