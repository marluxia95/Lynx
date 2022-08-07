#ifndef SCENE_H
#define SCENE_H

#include <stdio.h>
#include <memory>
#include <vector>
#include <stack>
#include "lynx_common.h"
#include "logger.h"
#include "entity.h"
// TODO

namespace Lynx {

    class Entity;

    class LYNXENGINE_API EntityManager {
    public:
        EntityManager();
        ~EntityManager();

        Entity* CreateEntity();
        Entity* CreateEntity(const char* name);

        template<class T>
        T* CreateEntity()
        {
            if(!std::is_base_of<Entity, T>())
                return nullptr;

            if(!m_id_stack.empty()){
                EntityID id = getCachedId();
                T* ent = new T(this, id);
                m_entities[id] = static_cast<Entity*>(ent);
            }
            T* new_ent = new T(this, m_entities.size()+1);
            m_entities.push_back(new_ent);

            return new_ent;
        }

        void RegisterEntity(Entity* entity);
        void UnregisterEntity(Entity* entity);

        bool EntityExists(EntityID entity);

        void Update();

    private:
        friend class Renderer;
        friend class Entity;
    protected:
        EntityID getCachedId();
        void deleteEntity(EntityID entity);
    protected:
        std::vector<Entity*> m_entities;
        std::stack<EntityID> m_id_stack;
    };

}

#endif
