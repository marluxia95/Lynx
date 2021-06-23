#ifndef PARENTINGSYSTEM_H
#define PARENTINGSYSTEM_H
#include <stdio.h>

#include "ECS/system.h"

namespace Lynx::ECS {

    class ParentingSystem : public System {
        public:
            void Init();
            void Update();
    };

}
#endif