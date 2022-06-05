#ifndef INPUT_HPP
#define INPUT_HPP

#include <glm/glm.hpp>
#include "lynx.h"

class Input {
public:
	Input() {

	}

	void movement() {
		std::shared_ptr<Lynx::Scene> scene = Lynx::GameApplication::GetGameInstance()->GetScene();

		LYNX_ASSERT(scene, "Fuck off");

		auto transformComponent = scene->GetComponent<Lynx::Transform>(camera);
		auto cameraComponent = scene->GetComponent<Lynx::Camera>(camera);

		float cameraSpeed = Lynx::GameApplication::GetGameInstance()->GetDeltaTime() * camera_Speed_Multiplier;

		if (Lynx::Input::IsKeyDown(GLFW_KEY_W))
			transformComponent->position += cameraSpeed * transformComponent->rotation;
		if (Lynx::Input::IsKeyDown(GLFW_KEY_S))
			transformComponent->position -= cameraSpeed * transformComponent->rotation;
		if (Lynx::Input::IsKeyDown(GLFW_KEY_A))
			transformComponent->position -= glm::normalize(glm::cross(transformComponent->rotation, cameraComponent->Up))* cameraSpeed;
		if (Lynx::Input::IsKeyDown(GLFW_KEY_D))
			transformComponent->position += glm::normalize(glm::cross(transformComponent->rotation, cameraComponent->Up))* cameraSpeed;
		if (Lynx::Input::IsKeyDown(GLFW_KEY_LEFT_SHIFT))
			camera_Speed_Multiplier = 5.0f;
		else
			camera_Speed_Multiplier = 3.0f;
	}

	void mouse_input() {
		std::shared_ptr<Lynx::Scene> scene = Lynx::GameApplication::GetGameInstance()->GetScene();

		auto transformComponent = scene->GetComponent<Lynx::Transform>(camera);
		auto cameraComponent = scene->GetComponent<Lynx::Camera>(camera);

		float xpos;
		float ypos;
		//if(Lynx::Input::IsJoystickConnected(GLFW_JOYSTICK_1)){
		//	mYaw += Lynx::Input::GetJoyAxis(GLFW_JOYSTICK_1, 0);
		//	mPitch += Lynx::Input::GetJoyAxis(GLFW_JOYSTICK_1, 1);
		//}else{
			if(!mouseActive){
				firstMouse = true;
				return;
			}
			glm::vec2 mpos = Lynx::Input::GetMousePos();
			xpos = mpos.x;
			ypos = mpos.y;

			if (firstMouse) {
				lastX = xpos;
				lastY = ypos;
				firstMouse = false;
			}

			float xoffset = xpos - lastX;
			float yoffset = lastY - ypos;
			lastX = xpos;
			lastY = ypos;

			xoffset *= sensitivity;
			yoffset *= sensitivity;

			mPitch += yoffset;
			mYaw += xoffset;
		//}

		

		if(mPitch > 89.9f) 
			mPitch = 89.9f;

		if(mPitch < -89.9f)
			mPitch = -89.9f;

		glm::vec3 direction;
		direction.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
		direction.y = sin(glm::radians(mPitch));
		direction.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
		transformComponent->rotation = glm::normalize(direction);
	}

	static int mouse_button_input(const Lynx::Event& ev);

	static int joystick_connected(const Lynx::Event& ev);

	static int joystick_disconnected(const Lynx::Event& ev);

private:
	float camera_Speed_Multiplier = 1.0f;
	bool firstMouse = true;
	float lastX, lastY;
	float sensitivity = 0.1f;
	float mPitch, mYaw;
	Lynx::EntityID camera;

	static bool mouseActive;
};

#endif