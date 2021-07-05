#include "eventManager.h"
#include "event.h"

namespace Lynx {


    void EventManager::Subscribe(EventType id, void (*func)(Event*))
    {
        listeners[id].push_back(func);
    }

    void EventManager::SendEvent(Event* event)
    {
        EventType type = event->GetType();

        for(auto const& listener : listeners[type]){
            listener(event);
        }
    }



}