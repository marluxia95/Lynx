#ifndef JOYSTICK_EVENT_H
#define JOYSTICK_EVENT_H

#include "Core/input.h"
#include "event.h"

namespace Lynx {

    class JoystickConnectedEvent : public Event {
        public:
            JoystickConnectedEvent(Joystick* joystick) : Event(JoystickConnected), joystick(joystick) {}
            Joystick* joystick;
    };

    class JoystickDisconnectedEvent : public Event {
        public:
            JoystickDisconnectedEvent(Joystick* joystick) : Event(JoystickDisconnected), joystick(joystick) {}
            Joystick* joystick;
    };

}

#endif