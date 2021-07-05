#ifndef EVENT_H
#define EVENT_H

#include <unordered_map>
#include <any>
#include <vector>

namespace Lynx {

    enum EventType {
        None = 0,
        WindowClose, WindowResize, Update, KeyPressed, MousePosCallback, MouseKeyPressed
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

        protected:
            EventType type;
    };

}


#endif