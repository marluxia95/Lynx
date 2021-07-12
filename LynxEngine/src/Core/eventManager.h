#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include <vector>
#include <unordered_map>
#include <GLFW/glfw3.h>
#include "Events/event.h"

namespace Lynx {
    
    class EventManager {
        
        using EventFunc = void (*)(const Event&);

        public:
            void Subscribe(const EventType& type, EventFunc&& func)
            {
                listeners[type].push_back(func);
            }
            void SendEvent(const Event& event)
            {
                EventType type = event.GetType();
                
                if(listeners.find(type) == listeners.end())
                    return;

                for(auto const& listener : listeners[type])
                    listener(event);
            }

        private:
            std::map<EventType, std::vector<EventFunc>> listeners;
    };

}

#endif