#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include <vector>
#include <unordered_map>
#include "Events/event.h"

namespace Lynx {

    class EventManager {

        public:
            void Subscribe(EventType id, void (*func)(Event*));
            void SendEvent(Event* event);

        private:
            std::unordered_map<EventType, std::vector<void(*)(Event*)>> listeners;
    };

}

#endif