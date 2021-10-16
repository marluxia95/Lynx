#include "luaAPI_core.h"

namespace Lynx::Lua {

    static int core_log_info(lua_State* L)
    {
        const char* string = luaL_checkstring(L, 1);
        log_info(string);
        return 1;
    }

    static int core_log_debug(lua_State* L)
    {
        const char* string = luaL_checkstring(L, 1);
        log_debug(string);
        return 1;
    }

    static int core_log_warn(lua_State* L)
    {
        const char* string = luaL_checkstring(L, 1);
        log_warn(string);
        return 1;
    }

    static int core_log_error(lua_State* L)
    {
        const char* string = luaL_checkstring(L, 1);
        log_error(string);
        return 1;
    }

    static int core_log_fatal(lua_State* L)
    {
        const char* string = luaL_checkstring(L, 1);
        log_fatal(string);
        return 1;
    }

    static const struct luaL_Reg lynx_core_lib [] = 
    {
        {"log_debug", core_log_debug},
        {"log_info",  core_log_info },
        {"log_warn",  core_log_warn },
        {"log_error", core_log_error},
        {"log_fatal", core_log_fatal},
        {NULL, NULL}
    };

    int luaopen_lynxcoreapi(lua_State* L)
    {
        lua_pushglobaltable(L);
        luaL_setfuncs(L, lynx_core_lib, 0);

        return 1;
    }

}