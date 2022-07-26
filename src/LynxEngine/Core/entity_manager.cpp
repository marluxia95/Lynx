#include "entity_manager.h"

namespace Lynx {

    EntityManager::EntityManager()
    {

    }

    EntityManager::~EntityManager()
    {
        for (auto e : m_entities) {
            free(e);
        }
    }

    Entity* EntityManager::CreateEntity()
    {
        Entity* newent = new Entity(m_entities.size());
        m_entities.push_back(newent);
        return newent;
    }

    Entity* EntityManager::CreateEntity(const char* name)
    {
        Entity* newent = new Entity(m_entities.size(), name);
        m_entities.push_back(newent);
        return newent;
    }


}
