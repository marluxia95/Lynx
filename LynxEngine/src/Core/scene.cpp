#include "Systems/cameraSystem.h"
#include "scene.h"

namespace Lynx {
    Entity Scene::CreateEntity(const char* name) {
        Entity ent = m_entityManager->CreateEntity();
        m_componentManager->AddComponent(ent, Generic{name});
        return ent;
    }

    Entity Scene::CreateEntity() {
        Entity ent = m_entityManager->CreateEntity();
        return ent;
    }

    void Scene::DestroyEntity(Entity entity) {
        m_entityManager->DestroyEntity(entity);
        EventManager::SendEvent(EntityDestroyedEvent{entity});
        m_componentManager->EntityDestroyed(entity);
    }
}