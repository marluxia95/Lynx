#ifndef ASYNC_H
#define ASYNC_H

#include <memory.h>
#include "event.h"

namespace Lynx {
    class AsyncTextureLoadEvent : public Event {
        public:
            AsyncTextureLoadEvent(std::shared_ptr<TextureBase> tex) : Event(AsyncTextureLoad), Tex(tex) {}
			std::shared_ptr<TextureBase> Tex;
    };
}

#endif
