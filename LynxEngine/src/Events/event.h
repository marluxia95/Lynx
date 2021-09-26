#ifndef EVENT_H
#define EVENT_H

#include <unordered_map>
#include <any>
#include <vector>
#include "Core/ECS/entity.h"

namespace Lynx {

    enum EventType {
        None = 0,
        WindowClose, WindowResize, KeyPressed, MousePosCallback, MouseKeyPressed, JoystickConnected, JoystickDisconnected,
        LastTick, UpdateTick, EngineInit, SignatureChanged, EntityDestroyed, Render
    };

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

    class SignatureChangedEvent : public Event {
        public:
            SignatureChangedEvent(Entity entity, Signature signature) : Event(SignatureChanged), entity(entity), signature(signature) {}

            Entity entity;
            Signature signature;
    };

    class EntityDestroyedEvent : public Event {
        public:
            EntityDestroyedEvent(Entity entity) : Event(EntityDestroyed), entity(entity) {}
            Entity entity;
    };

}


#endif