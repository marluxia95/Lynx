#ifndef CAMERASYSTEM_H
#define CAMERASYSTEM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Core/ECS/system.h"
#include "Core/ECS/components.h"

using namespace glm;

namespace Lynx {

	enum CameraType {
		CAMERA_ORTHOGRAPHIC,
		CAMERA_PERSPECTIVE
	};

	// Camera component : Contains all camera data so any other external component has access to it
	class Camera : ComponentBase {
	public:
		Camera() : FOV(60), Type(CAMERA_PERSPECTIVE), IsMain(true) {}
		Camera(vec2 resolution) : Resolution(resolution), FOV(60), Type(CAMERA_PERSPECTIVE), IsMain(true) {}
		float FOV = 60;
		vec2 Resolution;
		CameraType Type;
		bool IsMain;
		vec3 Up    = vec3(0, 1, 0);
		mat4 View  = mat4(1.0f);
		mat4 Projection = mat4(1.0f);
	};

	class CameraSystem : public ECS::System {
		public:
			CameraSystem(std::shared_ptr<Lynx::Scene> scene) : ECS::System(scene), cameraComponent() {}
			void Init();
			void Update();
			void CalculateProjections();
			void OnEntityAdded(EntityID ent) {}
            void OnEntityRemoved(EntityID ent) {}
		private:
			mat4 GetProjection(Camera* camera);
			mat4 GetView(EntityID entity, Camera* camera);
			Camera cameraComponent;
	};

}

#endif 
