#ifndef SCENE_H
#define SCENE_H

#include <stdio.h>
#include <memory>
#include <vector>
#include "entity.h"
#include "lynx_common.h"

// TODO 

namespace Lynx {

    class LYNXENGINE_API Scene {
    public: 
        Scene();
        ~Scene();

        EntityHandlePtr CreateEntity();
        EntityHandlePtr CreateEntity(const char* name);

    private:
        friend class Renderer;
    protected:
        std::vector<EntityHandlePtr> entities;
    };

}

#endif