#ifndef SCENE_H
#define SCENE_H

#include <stdio.h>
#include <memory>
#include <vector>
#include <stack>
#include "entity.h"
#include "lynx_common.h"

// TODO

namespace Lynx {

    class LYNXENGINE_API EntityManager {
    public:
        EntityManager();
        ~EntityManager();

        Entity* CreateEntity();
        Entity* CreateEntity(const char* name);

        bool EntityExists(EntityID entity);
    private:
        friend class Renderer;
        friend class Entity;
    protected:
        void deleteEntity(EntityID entity);
    protected:
        std::vector<Entity*> m_entities;
        std::stack<EntityID> m_id_stack;
    };

}

#endif
