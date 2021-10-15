#include "Core/application.h"
#include "luaRuntime.h"


namespace Lynx {

    int test(lua_State* state)
    {
        log_info("Hello from lua!");
        return 1;
    }

    LuaRuntime::LuaRuntime(Scene* scene) : ECS::System(scene) 
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
        lua_register(state, "test", test);
        log_debug("Lua initialized");
    }

    void LuaRuntime::Update()
    {


    }

    void LuaRuntime::lua_error(const char* file)
    {
        log_error("LUA Error in %s : %s", file, lua_tostring(state,-1));
    }

    void LuaRuntime::OnEntityAdded(Entity ent)
    {
        auto lua_component = scene->GetComponent<LuaScript>(ent);
        const char* file_path = lua_component->path;

        int success;

        success = luaL_loadfile(state, file_path);

        if(success != LUA_OK) {
            lua_error(file_path);
            return;
        }

        log_debug("LUA : Running file %s", file_path);

        success = lua_pcall(state, 0, LUA_MULTRET, 0);

        if(success != LUA_OK) {
            lua_error(file_path);
            return;
        }
    }

    void LuaRuntime::OnEntityRemoved(Entity ent)
    {

    }

    
}