#ifndef EVENT_H
#define EVENT_H

#include <unordered_map>
#include <any>
#include <vector>

namespace Lynx {

    enum EventType {
        None = 0,
        WindowClose, WindowResize, KeyPressed, MousePosCallback, MouseKeyPressed,
        LastTick, UpdateTick, EngineInit, Render
    };

    class Event {
        public:
            Event(EventType type): type(type)
            {

            };

            EventType GetType()
            {
                return type;
            }

        protected:
            EventType type;
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