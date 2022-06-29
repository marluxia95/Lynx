#include "Core/application.h"
#include "Core/event_manager.h"
#include "Events/event.h"
#include "lua_core.h"
#include "lua_events.h"
#include "lua_runtime.h"

namespace Lynx::Lua {

    static int core_log_info(lua_State* L)
    {
        const char* string = luaL_checkstring(L, 1);
        log_info(string);
        return 0;
    }

    static int core_log_debug(lua_State* L)
    {
        const char* string = luaL_checkstring(L, 1);
        log_debug(string);
        return 0;
    }

    static int core_log_warn(lua_State* L)
    {
        const char* string = luaL_checkstring(L, 1);
        log_warn(string);
        return 0;
    }

    static int core_log_error(lua_State* L)
    {
        const char* string = luaL_checkstring(L, 1);
        log_error(string);
        return 0;
    }

    static int core_log_fatal(lua_State* L)
    {
        const char* string = luaL_checkstring(L, 1);
        log_fatal(string);
        return 0;
    }

    static int core_application_getDeltaTime(lua_State* L)
    {
        lua_pushnumber(L, Application::GetSingleton()->GetDeltaTime());
        return 1;
    }

    static int core_application_getScene(lua_State* l)
    {
        
        return 1;
    }

    static void core_event(const Event& ev) {

    }

    static int core_addEventListener(lua_State* L)
    {
        int argcount = lua_gettop(L);

        if(argcount != 2){
            log_warn("LUA : AddEventListener must be called with 2 arguments");
            return 0;
        }

        const char* event_str = lua_tostring(L, 1);

        EventType event_type = StringToEventMap[event_str];
        
        //log_debug("LUA AddEventListener() : Event %s %d", event_str, event_type);

        int func;

        if(lua_isfunction(L, 2)) {
            lua_newtable(L);
            lua_pushvalue(L, 2);
            func = luaL_ref(L, LUA_REGISTRYINDEX);
        }else{
            log_error("LUA : AddEventListener argument 2 must be a function !");
            return -1;
        }

        EventManager::AddListener(event_type, [L, func](const Event& ev){
            //log_debug("LUA : Got event");
            EventType type = ev.GetType();
            lua_rawgeti(L, LUA_REGISTRYINDEX, func);
            EventToTable(L, type, ev);
            //log_debug("LUA : Calling event function");
            int success;
            if(lua_isfunction(L, 1)) {
                success = lua_pcall(L, 1, 0, 0);

                if(success != LUA_OK) {
                    //LuaError(L);
                    luaL_unref(L, LUA_REGISTRYINDEX, func);
                    return;
                }
            }else{
                log_warn("LUA : EventManager::AddListener() Argument is not a function");
            }
        });

        EventManager::AddListener(LastTick, [L, func](const Event&ev){
            luaL_unref(L, LUA_REGISTRYINDEX, func);
        });
        return 0;
    }

    static const struct luaL_Reg lynx_core_lib [] = 
    {
        {"log_debug", core_log_debug},
        {"log_info",  core_log_info },
        {"log_warn",  core_log_warn },
        {"log_error", core_log_error},
        {"log_fatal", core_log_fatal},
        {"AddEventListener", core_addEventListener},
        {NULL, NULL}
    };

    static const struct luaL_Reg lynx_application [] = 
    {
        {"GetDeltaTime", core_application_getDeltaTime},
        {"GetScene", core_application_getScene},
        {NULL, NULL}
    };

    int luaopen_lynxcoreapi(lua_State* L)
    {
        lua_pushglobaltable(L);
        luaL_setfuncs(L, lynx_core_lib, 0);

        lua_newtable(L);
        luaL_setfuncs(L, lynx_application, 0);
        lua_setglobal(L, "Application");

        return 1;
    }

}