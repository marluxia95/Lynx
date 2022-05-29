#include "Core/application.h"
#include "luaRuntime.h"

#include "luaAPI_core.h"

namespace Lynx::Lua {


    LuaRuntime::LuaRuntime(std::shared_ptr<Lynx::Scene> scene) : ECS::System(scene) 
    {
        

    }

    LuaRuntime::~LuaRuntime()
    {
        lua_close(state);
    }

    void LuaRuntime::Init()
    {
        state = luaL_newstate();
        luaL_openlibs(state);
        {
            luaL_requiref(state, "LynxCore", luaopen_lynxcoreapi, 1);
            lua_pop(state, 1);
        }
    }

    void LuaRuntime::Update()
    {


    }
    void LuaError(lua_State* L)
    {
        log_error("%s", lua_tostring(L,-1));
    }

    void LuaRuntime::Load(char* path)
    {
        int success;

        success = luaL_loadfile(state, path);

        if(success != LUA_OK) {
            LuaError(state);
            return;
        }

        success = lua_pcall(state, 0, LUA_MULTRET, 0);

        if(success != LUA_OK) {
            LuaError(state);
            return;
        }
    }
    
}