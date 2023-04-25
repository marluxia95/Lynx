#ifndef EVENT_H
#define EVENT_H

#include <unordered_map>
#include <any>
#include <string>
#include <vector>
#include <map>
#include "lynx_common.h"

namespace Lynx {

    enum EventType : unsigned int {
        None = 0,
        WindowClose, WindowResize, KeyPressed, MousePosCallback, MouseKeyPressed, JoystickConnected, JoystickDisconnected,
        FirstTick, LastTick, UpdateTick, EngineInit, Render, AsyncTextureLoad
    };

    extern std::map<std::string, EventType> StringToEventMap;

    const char* GetEventString(EventType type);

    class Event {
        public:
            Event(EventType type): type(type)
            {

            };

            EventType GetType() const
            {
                return type;
            } 

        protected:
            EventType type;
    };

    class FirstTickEvent : public Event {
        public:
            FirstTickEvent() : Event(FirstTick) {}

    };

    class LastTickEvent : public Event {
        public:
            LastTickEvent() : Event(LastTick) {}

    };

    class UpdateTickEvent : public Event {
        public:
            UpdateTickEvent() : Event(UpdateTick) {}
    };

    class InitEvent : public Event {
        public: 
            InitEvent() : Event(EngineInit) {}
    };

    class RenderEvent : public Event {
        public: 
            RenderEvent() : Event(Render) {}
    };

}


#endif
