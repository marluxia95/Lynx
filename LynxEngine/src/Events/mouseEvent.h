#ifndef MOUSEEVENT_H
#define MOUSEEVENT_H

#include <glm/glm.hpp>
#include "event.h"
#include "keyEvent.h"

namespace Lynx {

    struct MousePosition {
        double x;
        double y;
    };
    
    class MouseButtonEvent : public Event {

        public:
            MouseButtonEvent(KeyCode keycode, const int action) : Event(MouseKeyPressed), m_keyCode(keycode), m_action(action) {}
            KeyCode m_keyCode;
            int m_action;
    };

    class MouseCallbackEvent : public Event  {

        public:
            MouseCallbackEvent(double posX, double posY) : Event(MousePosCallback) { 
                m_pos.x = posX; 
                m_pos.y = posY; 
            }
            
            MousePosition m_pos;
    };

}




#endif