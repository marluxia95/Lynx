#include "entity_manager.h"
#include "entity.h"
#include "event_manager.h"
#include "Graphics/renderer.h"
#include "application.h"

namespace Lynx {

    EntityManager::EntityManager()
    {
        EventManager::AddListener(UpdateTick, [this](const Event& ev){
                Update();
        });
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
        Entity* newent = new Entity(this, m_entities.size()+1);
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
        if(entity > m_entities.size())
            return false;

        return m_entities[entity] != nullptr;
    }

    void EntityManager::RegisterEntity(Entity* entity)
    {
        // Reuse unused slots from deleted entities
        if(!m_id_stack.empty()){
            EntityID id = getCachedId();
            entity->m_id = id;
            m_entities[id] = entity;
            return;
        }

        entity->m_id = m_entities.size()+1;
        m_entities.push_back(entity);
    }

    void EntityManager::UnregisterEntity(Entity* entity)
    {
        deleteEntity(entity->m_id);
    }

    void EntityManager::deleteEntity(EntityID entity_id)
    {
        m_id_stack.push(entity_id);
        m_entities[entity_id] = nullptr;
    }

    void EntityManager::Update()
    {
        for(auto ent : m_entities) {
            if(ent == nullptr)
                continue;

            ent->UpdatePhysics();
            ent->Think();
            Application::GetSingleton()->GetRenderer()->PushRender(ent);
        }
    }

    EntityID EntityManager::getCachedId()
    {
        EntityID id = m_id_stack.top();
        m_id_stack.pop();
        return id;
    }
}
