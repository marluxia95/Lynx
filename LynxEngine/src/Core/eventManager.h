#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include <vector>
#include <unordered_map>
#include "Events/event.h"

namespace Lynx {
    
    class EventManager {

        public:
            void Subscribe(EventType id, void (*func)(Event*))
            {
                listeners[id].push_back(func);
            }
            void SendEvent(Event* event)
            {
                EventType type = event->GetType();

                for(auto const& listener : listeners[type]){
                    listener(event);
                }

                free(event);
            }

        private:
            std::unordered_map<EventType, std::vector<void(*)(Event*)>> listeners;
    };

}

#endif