#ifndef LIGHTINGSYSTEM_H
#define LIGHTINGSYSTEM_H

#include <stdio.h>
#include "Core/ECS/system.h"

namespace Lynx {

class LightingSystem : public ECS::System {
    public:
        LightingSystem(Scene* scene) : ECS::System(scene) {}
        void Init() {}
        void Update() {}
        void OnEntityAdded(Entity ent) {}
        void OnEntityRemoved(Entity ent) {}

};

}

#endif
