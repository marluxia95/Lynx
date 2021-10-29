#include <iostream>
#include <memory>
#include "eventManager.h"
#include "Core/logger.h"

namespace Lynx {

std::map<EventType, std::vector<EventManager::EventFunc>> EventManager::listeners = std::map<EventType, std::vector<EventFunc>>();


void EventManager::AddListener(const EventType& type, EventFunc&& func)
{
    listeners[type].push_back(func);
}

void EventManager::SendEvent(const Event& event)
{
    EventType type = event.GetType();
                
    if(listeners.find(type) == listeners.end())
        return;

    for(int i = 0; i < listeners[type].size(); i++){
        auto listener = listeners[type][i];

        if(listener(event) < 0 ){ // Remove listener
            listeners[type].erase(listeners[type].begin() + i);
            log_debug("EventManager : Removed index %d New Size : %d", i, listeners[type].size());
        }
    }
}

}