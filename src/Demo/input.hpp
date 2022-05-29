#pragma once

#include <glm/glm.hpp>
#include "lynx.h"

extern Lynx::GameApplication* applicationInstance;

float camera_Speed_Multiplier = 1.0f;
bool firstMouse = true;
float lastX, lastY;
float sensitivity = 0.1f;
float mPitch, mYaw;
bool mouseActive = false;
Lynx::EntityID camera;

void movement()
{
	std::shared_ptr<Lynx::Scene> scene = applicationInstance->GetScene();
	auto transformComponent = scene->GetComponent<Lynx::Transform>(camera);
	auto cameraComponent = scene->GetComponent<Lynx::Camera>(camera);

	float cameraSpeed = applicationInstance->GetDeltaTime() * camera_Speed_Multiplier;

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

	//if(Lynx::Input::IsJoystickConnected(GLFW_JOYSTICK_1)){
	//	transformComponent->position += cameraSpeed * transformComponent->rotation * Lynx::Input::GetJoyAxis(GLFW_JOYSTICK_1, 4);
	//	transformComponent->position += cameraSpeed * transformComponent->rotation * Lynx::Input::GetJoyAxis(GLFW_JOYSTICK_1, 3);
	//}
}

void mouse_input()
{
	std::shared_ptr<Lynx::Scene> scene = applicationInstance->GetScene();
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

int mouse_button_input(const Lynx::Event& ev)
{
	const Lynx::MouseButtonEvent& button_event = static_cast<const Lynx::MouseButtonEvent&>(ev);
	if(button_event.m_keyCode == GLFW_MOUSE_BUTTON_2 && button_event.m_action == GLFW_PRESS){
		mouseActive = true;
		glfwSetInputMode(Lynx::GameApplication::GetGameInstance()->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}else if(button_event.m_action == GLFW_RELEASE){
		mouseActive = false;
		glfwSetInputMode(Lynx::GameApplication::GetGameInstance()->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	return 1;
}

int joystick_connected(const Lynx::Event& ev)
{
	const Lynx::JoystickConnectedEvent& joy_event = static_cast<const Lynx::JoystickConnectedEvent&>(ev);

	log_info("Joystick connected : %s ( ID : %d )", joy_event.joystick->name, joy_event.joystick->id);
	
	return 1;
}

int joystick_disconnected(const Lynx::Event& ev)
{
	const Lynx::JoystickDisconnectedEvent& joy_event = static_cast<const Lynx::JoystickDisconnectedEvent&>(ev);

	log_info("Joystick disconnected : %s", joy_event.joystick->name);

	return 1;
}
