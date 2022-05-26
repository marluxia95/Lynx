#ifndef EVENT_H
#define EVENT_H

#include <unordered_map>
#include <any>
#include <string>
#include <vector>
#include <map>
#include "Core/ECS/entity.h"
#include "lynx_common.h"

namespace Lynx {

    enum EventType {
        None = 0,
        WindowClose, WindowResize, KeyPressed, MousePosCallback, MouseKeyPressed, JoystickConnected, JoystickDisconnected,
        FirstTick, LastTick, UpdateTick, EngineInit, SignatureChanged, EntityDestroyed, Render, AsyncTextureLoad
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

    class SignatureChangedEvent : public Event {
        public:
            SignatureChangedEvent(EntityID entity, Signature signature) : Event(SignatureChanged), entity(entity), signature(signature) {}
           
            EntityID entity;
            Signature signature;
    };

    class EntityDestroyedEvent : public Event {
        public:
            EntityDestroyedEvent(EntityID entity) : Event(EntityDestroyed), entity(entity) {}
            EntityID entity;
    };

}


#endif