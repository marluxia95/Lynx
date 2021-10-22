#include "luaAPI_events.h"
#include "Core/logger.h"
#include "Events/joystickEvent.h"
#include "Events/windowEvent.h"
#include "Events/mouseEvent.h"
#include "Events/keyEvent.h"

namespace Lynx::Lua {

    void EventToTable(lua_State* L, EventType ev, const Event& r_event)
    {        
        log_debug("Creating table");
        lua_newtable(L);
        log_debug("Table created");
        lua_pushstring(L, "Type");
        lua_pushstring(L, GetEventString(r_event.GetType()) );
        log_debug("Pushed data");
        lua_settable(L, -3);

        switch (ev) {
            case SignatureChanged:
                {
                    const SignatureChangedEvent& event = static_cast<const SignatureChangedEvent&>(r_event);
                    lua_pushstring(L, "Entity");
                    lua_pushnumber(L, event.entity);
                    lua_settable(L, -3);
                }
                break;
            case EntityDestroyed:
                {
                    const EntityDestroyedEvent& event = static_cast<const EntityDestroyedEvent&>(r_event);
                    lua_pushstring(L, "Entity");
                    lua_pushnumber(L, event.entity);
                    lua_settable(L, -3);
                }
                break;
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
        
        log_debug("Set table information");
    }

}