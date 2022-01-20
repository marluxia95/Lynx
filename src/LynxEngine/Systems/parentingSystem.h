#ifndef PARENTINGSYSTEM_H
#define PARENTINGSYSTEM_H
#include <stdio.h>

#include "Core/ECS/system.h"

namespace Lynx::ECS {

    class ParentingSystem : public System {
        public:
            ParentingSystem(std::shared_ptr<Lynx::Scene> scene) : System(scene) {}
            void OnEntityAdded(EntityID ent) {}
            void OnEntityRemoved(EntityID ent) {}
            void Init();
            void Update();
    };

}
#endif