#include "application.h"
#include "eventManager.h"
#include "Events/event.h"
#include "Systems/cameraSystem.h"
#include "scene.h"

namespace Lynx {

    Scene::Scene() : m_componentManager(Application::GetInstance()->GetComponentManager())
    {
        log_debug("Created scene");
        m_entityManager = std::make_unique<ECS::EntityManager>();
    }

    EntityID Scene::CreateEntity(const char* name) {
        EntityID ent = m_entityManager->CreateEntity();
        m_componentManager->AddComponent(ent, Generic{name});
        log_debug("Scene : Created entity %ld", ent);
        return ent;
    }

    EntityID Scene::CreateEntity() {
        EntityID ent = m_entityManager->CreateEntity();
        log_debug("Scene : Created entity %ld", ent);
        return ent;
    }

    void Scene::DestroyEntity(EntityID entity) {
        m_entityManager->DestroyEntity(entity);
        EventManager::SendEvent(EntityDestroyedEvent{entity});
        m_componentManager->EntityDestroyed(entity);
    }

    void Scene::updateSignature(EntityID entity, Signature signature)
    {
        m_entityManager->SetSignature(entity, signature);
        EventManager::SendEvent(SignatureChangedEvent{entity, signature});
    }

}