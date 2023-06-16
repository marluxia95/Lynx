#include <stdbool.h>
#include <GLFW/glfw3.h>
#include "logger.h"
#include "input.h"
#include "event_manager.h"
#include "Events/keyEvent.h"
#include "Events/mouseEvent.h"
#include "Events/joystickEvent.h"

namespace Lynx {

	extern GLFWwindow* window;

	int Input::keysUp[GLFW_KEY_LAST] = {};
	int Input::keysDown[GLFW_KEY_LAST] = {};
	Joystick* Input::connectedJoysticks[GLFW_JOYSTICK_LAST] = {};
	glm::dvec2 Input::mouse_pos = glm::dvec2();

	bool Input::IsJoystickConnected(int joyid)
	{
		if(joyid > GLFW_JOYSTICK_LAST | joyid < 0)
			return false;

		if(connectedJoysticks[joyid])
			return true;

		return false;
	}

	bool Input::GetJoyButton(int joyid, int button)
	{
		int c;
		if(button > GLFW_GAMEPAD_BUTTON_LAST | button < 0)
			return false;

		if(connectedJoysticks[joyid]){
			if(!connectedJoysticks[joyid]->isGamePad)
				return glfwGetJoystickButtons(joyid, &c)[button];

			if (glfwGetGamepadState(joyid, (GLFWgamepadstate*)&connectedJoysticks[joyid]->gpadstate))
				return connectedJoysticks[joyid]->gpadstate.buttons[button];
		}

		return false;
	}

	float Input::GetJoyAxis(int joyid, int axis)
	{
		int c = 0;
		if(axis > GLFW_GAMEPAD_AXIS_LAST | axis < 0)
			return 0.0f;

		if(connectedJoysticks[joyid]){
			if(!connectedJoysticks[joyid]->isGamePad)
				return glfwGetJoystickAxes(joyid, &c)[axis];
			

			if (glfwGetGamepadState(joyid, (GLFWgamepadstate*)&connectedJoysticks[joyid]->gpadstate))
				return connectedJoysticks[joyid]->gpadstate.axes[axis];

		}

		return 0.0f;
	}

	bool Input::IsKeyDown(int kname)
	{
		if(kname < GLFW_KEY_LAST)
			return keysDown[kname];
		else
			return false;
	}

	bool Input::IsKeyUp(int kname)
	{
		if(kname < GLFW_KEY_LAST)
			return keysUp[kname];
		else
			return false;
	}

	void Input::Init()
	{
#ifdef JOYSTICK_SUPPORT
		glfwSetJoystickCallback(joystick_callback);
		
		for(int j = 0; j < GLFW_JOYSTICK_LAST; j++){
			if(glfwJoystickPresent(j)){
				addJoystick(j);
			}
		}
#endif
		EventManager::AddListener(KeyPressed, UpdateKeys);
		EventManager::AddListener(MousePosCallback, mouse_callback);

		log_debug("Input manager intialized");
	}

	glm::dvec2 Input::GetMousePos()
	{
		return mouse_pos;
	}

	void Input::addJoystick(int id)
	{
		connectedJoysticks[id] = new Joystick;
		connectedJoysticks[id]->id = id;
		connectedJoysticks[id]->name = glfwGetJoystickName(id);
		glfwGetJoystickAxes(id, &connectedJoysticks[id]->axisCount);
		glfwGetJoystickButtons(id, &connectedJoysticks[id]->buttonCount);
		glfwGetJoystickHats(id, &connectedJoysticks[id]->hatCount);
		connectedJoysticks[id]->ptr = glfwGetJoystickUserPointer(id);
		EventManager::SendEvent(JoystickConnectedEvent(connectedJoysticks[id]));
		log_info("Joystick %d %s connected", id, connectedJoysticks[id]->name);
	}

	void Input::joystick_callback(int id, int ev)
	{
		if (ev == GLFW_CONNECTED && !connectedJoysticks[id])
		{
			addJoystick(id);
		}
		else if (ev == GLFW_DISCONNECTED && connectedJoysticks[id])
		{
			EventManager::SendEvent(JoystickDisconnectedEvent(connectedJoysticks[id]));
			delete connectedJoysticks[id];
		}
	}

	void Input::UpdateKeys(const Event& ev)
	{
		const KeyPressedEvent& key_event = static_cast<const KeyPressedEvent&>(ev);
		if(key_event.m_keyCode > 1024)
			return;

		if(key_event.m_action == GLFW_PRESS){
			keysDown[key_event.m_keyCode] = true;
			keysUp[key_event.m_keyCode] = false;
		}else if(key_event.m_action == GLFW_RELEASE) {
			keysUp[key_event.m_keyCode] = true;
			keysDown[key_event.m_keyCode] = false;
		}
	}

	void Input::mouse_callback(const Event& ev)
	{
		const MouseCallbackEvent& mouse_event = static_cast<const MouseCallbackEvent&>(ev);
		mouse_pos.x = mouse_event.m_pos.x;
		mouse_pos.y = mouse_event.m_pos.y;
	}

	void Input::EnableCursor(bool enable)
	{
		long int status = enable ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL;
		glfwSetInputMode(window, GLFW_CURSOR, status);
	}


	Input::~Input()
	{
		for(int j = 0; j < GLFW_JOYSTICK_LAST; j++) {
			if(connectedJoysticks[j])
				delete connectedJoysticks[j];
		}
	}

	

}
