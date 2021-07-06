#ifndef KEYEVENT_H
#define KEYEVENT_H

#include <GLFW/glfw3.h>
#include "event.h"

using KeyCode = uint16_t;

namespace Lynx {

class KeyEvent : public Event {
    public:
        KeyCode GetKeyCode() { return m_keyCode; }

    protected:
        KeyEvent(const KeyCode keycode, EventType type) : Event(type), m_keyCode(keycode) {}
        KeyCode m_keyCode;

};

class KeyPressedEvent : public KeyEvent {
    public:
        KeyPressedEvent(const KeyCode keycode) 
            : KeyEvent(keycode, KeyPressed) {}

};

}

#endif