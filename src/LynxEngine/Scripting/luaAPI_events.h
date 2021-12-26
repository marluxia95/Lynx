#ifndef LYNX_LUAAPI_EVENTS_H
#define LYNX_LUAAPI_EVENTS_H 

#include "luaCommon.h"
#include "Events/event.h"


namespace Lynx::Lua {

    void EventToTable(lua_State* L, EventType ev, const Event& r_event);


}

#endif // LYNX_LUAAPI_EVENTS_H