#ifndef KEYEVENT_H
#define KEYEVENT_H

#include <GLFW/glfw3.h>
#include "event.h"

using KeyCode = uint16_t;

namespace Lynx {

class KeyEvent : public Event {
    public:
        KeyCode m_keyCode;
        int m_action;
        
    protected:
        KeyEvent(const KeyCode keycode, const int action, EventType type) : Event(type), m_keyCode(keycode), m_action(action) {}
        

};

class KeyPressedEvent : public KeyEvent {
    public:
        KeyPressedEvent(const KeyCode keycode, const int action) 
            : KeyEvent(keycode, action, KeyPressed) {}

};

}

#endif