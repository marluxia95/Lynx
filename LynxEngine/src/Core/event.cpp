#include "event.h"

namespace Lynx {


    void Event::Subscribe(EventType id, void (*func)(EventType*))
    {
        listeners[id].push_back(func);
    }

    void Event::SendEvent(Event* event)
    {
        int type = event->GetType();

        for(auto const& listener : listeners[type]){
            listener(event);
        }
    }



}