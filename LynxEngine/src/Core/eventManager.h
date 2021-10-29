#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include <vector>
#include <unordered_map>
#include <map>
#include <functional>
#include "Events/event.h"

namespace Lynx {
    
    class EventManager {

        public:

            using EventFunc = std::function<int(const Event&)>;

            /*
                Adds a listener to an event
            */
            static void AddListener(const EventType& type, EventFunc&& func);

            /*
                Sends an event signal
            */
            static void SendEvent(const Event& event);

        private:
            static std::map<EventType, std::vector<EventFunc>> listeners;
    };

}

#endif