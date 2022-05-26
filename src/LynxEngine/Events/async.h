#ifndef ASYNC_H
#define ASYNC_H

#include "event.h"

namespace Lynx {
    class AsyncTextureLoadEvent : public Event {
        public:
            AsyncTextureLoadEvent(Graphics::TextureBase* tex) : Event(AsyncTextureLoad), Tex(tex) {}
			Graphics::TextureBase* Tex;
    };
}

#endif