#include "entity_manager.h"

namespace Lynx {

    EntityManager::EntityManager()
    {

    }

    EntityManager::~EntityManager()
    {
        for (auto ent : m_entities) {
            if(ent != nullptr) 
                delete ent;
        }
    }

    Entity* EntityManager::CreateEntity()
    {
        if(!m_id_stack.empty()){
            EntityID id = m_id_stack.top();
            m_id_stack.pop();
            return m_entities[id] = new Entity(this, id);
        }
        Entity* newent = new Entity(this, m_entities.size());
        m_entities.push_back(newent);
        return newent;
    }

    Entity* EntityManager::CreateEntity(const char* name)
    {
        Entity* newent = CreateEntity();
        newent->SetName(name);
        return newent;
    }

    bool EntityManager::EntityExists(EntityID entity)
    {
        return m_entities[entity] != nullptr;
    }

    void EntityManager::deleteEntity(EntityID entity_id)
    {
        m_id_stack.push(entity_id);
        m_entities[entity_id] = nullptr;
    }
}
