#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H


#include <stdio.h>
#include "system.h"

namespace Lynx {

class RenderSystem : ECS::System {
    public:
        void Init();
        void Update();
};

}

#endif