#ifndef LYNX_LUARUNTIME_H
#define LYNX_LUARUNTIME_H

#include "luaCommon.h"
#include "Core/logger.h"
#include "Core/ECS/system.h"
#include "lynx_common.h"

namespace Lynx::Lua {

    class LYNXENGINE_API LuaRuntime : public ECS::System {
        public:
            LuaRuntime(std::shared_ptr<Lynx::Scene> scene);
            ~LuaRuntime();
            void Init();
            void Update();
            void Load(char* script);
        private:
            lua_State* state;
    };

    void LuaError(lua_State* L);

}

#endif