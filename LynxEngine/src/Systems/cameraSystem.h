#ifndef CAMERASYSTEM_H
#define CAMERASYSTEM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Core/ECS/system.h"

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
		vec3 up    = vec3(0, 1, 0);
		mat4 view  = mat4(1.0f);
		mat4 projection = mat4(1.0f);
	};

	class CameraSystem : public ECS::System {
		public:
			CameraSystem(Scene* scene) : ECS::System(scene) {}
			void Init();
			void Update();
			void CalculateProjections();
			void OnEntityAdded(Entity ent) {}
            void OnEntityRemoved(Entity ent) {}
		private:
			mat4 GetProjection(Camera* camera);
			mat4 GetView(Entity entity, Camera* camera);
			Camera cameraComponent;
	};

}

#endif 
