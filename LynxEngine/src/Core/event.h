#ifndef EVENT_H
#define EVENT_H

#include <unordered_map>
#include <vector>

namespace Lynx {

    enum EventType {
        None = 0,
        WindowClose, WindowResize, Update, KeyPressed
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

        private:
            EventType type;
    };

    class EventManager {

        public:
            void Subscribe(EventType id, void (*func)(EventType*));
            void SendEvent(Event* event);

        private:
            std::unordered_map<EventType, std::vector<void(*)(EventType*)>> listeners;
    };


}


#endif