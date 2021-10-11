#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include <vector>
#include <unordered_map>
#include <map>
#include <functional>
#include "Events/event.h"

namespace Lynx {
    
    class EventManager {
        
        using EventFunc = std::function<void(const Event&)>;

        public:
            /*
                Adds a listener to an event
            */
            static void AddListener(const EventType& type, EventFunc&& func)
            {
                listeners[type].push_back(func);
            }

            /*
                Sends an event signal
            */
            static void SendEvent(const Event& event)
            {
                EventType type = event.GetType();
                
                if(listeners.find(type) == listeners.end())
                    return;

                for(auto const& listener : listeners[type])
                    listener(event);
            }

        private:
            static std::map<EventType, std::vector<EventFunc>> listeners;
    };

}

#endif