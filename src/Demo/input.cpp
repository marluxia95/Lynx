#include "input.h"

bool Input::mouseActive = false;

int Input::mouse_button_input(const Lynx::Event& ev)
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

int Input::joystick_connected(const Lynx::Event& ev)
{
	const Lynx::JoystickConnectedEvent& joy_event = static_cast<const Lynx::JoystickConnectedEvent&>(ev);

	log_info("Joystick connected : %s ( ID : %d )", joy_event.joystick->name, joy_event.joystick->id);
		
	return 1;
}

int Input::joystick_disconnected(const Lynx::Event& ev)
{
	const Lynx::JoystickDisconnectedEvent& joy_event = static_cast<const Lynx::JoystickDisconnectedEvent&>(ev);

	log_info("Joystick disconnected : %s", joy_event.joystick->name);

	return 1;
}