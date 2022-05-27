#ifndef ASYNC_H
#define ASYNC_H

#include <memory.h>
#include "event.h"

namespace Lynx {
    class AsyncTextureLoadEvent : public Event {
        public:
            AsyncTextureLoadEvent(std::shared_ptr<Graphics::TextureBase> tex) : Event(AsyncTextureLoad), Tex(tex) {}
			std::shared_ptr<Graphics::TextureBase> Tex;
    };
}

#endif