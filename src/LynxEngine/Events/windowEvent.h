#ifndef WINDOWEVENT_H
#define WINDOWEVENT_H

#include "event.h"

namespace Lynx {

    class WindowResizeEvent : public Event {
        public:
            WindowResizeEvent(int width, int height) 
                : m_width(width), m_height(height), Event(WindowResize)
                { }

            int GetWidth() { return m_width; }
            int GetHeight() { return m_height; }

        private:
            int m_width, m_height;
    };

}


#endif