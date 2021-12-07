#ifndef LYNX_LUARUNTIME_H
#define LYNX_LUARUNTIME_H

#include "luaCommon.h"
#include "Core/logger.h"
#include "Core/ECS/system.h"

namespace Lynx::Lua {

    class LuaRuntime : public ECS::System {
        public:
            LuaRuntime(Scene* scene);
            ~LuaRuntime();
            void Init();
            void Update();
            void OnEntityAdded(EntityID ent);
            void OnEntityRemoved(EntityID ent);
        private:
            lua_State* state;
    };

    void LuaError(lua_State* L);

}

#endif