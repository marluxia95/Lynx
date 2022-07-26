#include "lua_events.h"
#include "Core/logger.h"
#include "Events/joystickEvent.h"
#include "Events/windowEvent.h"
#include "Events/mouseEvent.h"
#include "Events/keyEvent.h"

namespace Lynx::Lua {

    void EventToTable(lua_State* L, EventType ev, const Event& r_event)
    {        
        lua_newtable(L);
        lua_pushstring(L, "Type");
        lua_pushstring(L, GetEventString(r_event.GetType()) );
        lua_settable(L, -3);

        switch (ev) {
            case KeyPressed:
                {
                    const KeyPressedEvent& event = static_cast<const KeyPressedEvent&>(r_event);
                    lua_pushstring(L, "KeyCode");
                    lua_pushnumber(L, event.m_keyCode);
                    lua_settable(L, -3);
                }
                break;
            case MouseKeyPressed:
                {
                    const MouseButtonEvent& event = static_cast<const MouseButtonEvent&>(r_event);
                    lua_pushstring(L, "KeyCode");
                    lua_pushnumber(L, event.m_keyCode);
                    lua_settable(L, -3);
                }
                break;
            case MousePosCallback:
                {
                    const MouseCallbackEvent& event = static_cast<const MouseCallbackEvent&>(r_event);
                    lua_pushstring(L, "x");
                    lua_pushnumber(L, event.m_pos.x);
                    lua_settable(L, -3);

                    lua_pushstring(L, "y");
                    lua_pushnumber(L, event.m_pos.x);
                    lua_settable(L, -3);
                }
                break;
        };
    }

}