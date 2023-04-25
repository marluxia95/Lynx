#ifndef LYNX_LUAAPI_EVENTS_H
#define LYNX_LUAAPI_EVENTS_H 

#include "Core/event_manager.h"
#include "lua_common.h"

namespace Lynx {

class Event;

namespace Lua {

    void EventToTable(lua_State* L, EventType ev, const Event& r_event);


}
}

#endif // LYNX_LUAAPI_EVENTS_H
