#ifndef PARENTINGSYSTEM_H
#define PARENTINGSYSTEM_H
#include <stdio.h>

#include "Core/ECS/system.h"

namespace Lynx::ECS {

    class ParentingSystem : public System {
        public:
            ParentingSystem(Scene* scene) : System(scene) {}
            void OnEntityAdded(Entity ent) {}
            void OnEntityRemoved(Entity ent) {}
            void Init();
            void Update();
    };

}
#endif