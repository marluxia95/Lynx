#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <stdbool.h>
#include <map>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "application.h"
#include "eventManager.h"

namespace Lynx {

    typedef struct {
        int id;
        const char* name;
        int axisCount;
        int buttonCount;
        int hatCount;
        bool isGamePad;
        GLFWgamepadstate gpadstate;
        void* ptr;
    } Joystick;

    class Input {
        public:
            ~Input();
            static bool IsJoystickConnected(int joyid);
            static float GetJoyAxis(int joyid, int axis);
            static bool GetJoyButton(int joyid, int axis);
            static bool IsKeyDown(int keycode);
            static bool IsKeyUp(int keycode);
            static glm::dvec2 GetMousePos();
            
            friend class Application;
            friend class GameApplication;
        private:
            static void Init();
            static void joystick_callback(int id, int ev);
            static int mouse_callback(const Event& ev);
            static int UpdateKeys(const Event& ev);
            static int keysUp[GLFW_KEY_LAST];
            static int keysDown[GLFW_KEY_LAST];
            static int isJoystickPresent;

            static void addJoystick(int id);
            static Joystick* connectedJoysticks[GLFW_JOYSTICK_LAST];
            
            static glm::dvec2 mouse_pos;
    };

}

#endif