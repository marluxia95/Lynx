#ifndef SCENE_H
#define SCENE_H

#include <stdio.h>
#include <memory>
#include "eventManager.h"
#include "Events/event.h"
#include "ECS/entity.h"
#include "ECS/entityManager.h"
#include "ECS/componentManager.h"
#include "ECS/components.h"

namespace Lynx {

    class GameApplication;
    
    class Scene {
        public:
            Scene(ECS::ComponentManager* componentManager) : m_componentManager(componentManager)
            {
                m_entityManager = std::make_unique<ECS::EntityManager>();
            }

            Entity CreateEntity(const char* name);
            Entity CreateEntity();
            void DestroyEntity(Entity entity);

            template<typename T>
            void RegisterComponent(){
                m_componentManager->RegisterComponent<T>();
            }

            template<typename T>
            void AddComponent(Entity entity, T component){
                m_componentManager->AddComponent(entity, component);

                auto signature = m_entityManager->GetSignature(entity);
                signature.set(m_componentManager->GetComponentType<T>(), true);
                m_entityManager->SetSignature(entity, signature);

                EventManager::SendEvent(SignatureChangedEvent{entity, signature});
            }

            template<typename T>
            void RemoveComponent(Entity entity){
                m_componentManager->RemoveComponent<T>(entity);

                auto signature = m_entityManager->GetSignature(entity);
                signature.set(m_componentManager->GetComponentType<T>(), false);
                m_entityManager->SetSignature(entity, signature);

                EventManager::SendEvent(SignatureChangedEvent{entity, signature});
            }

            template<typename T>
            T* GetComponent(Entity entity){
                return m_componentManager->GetComponent<T>(entity);
            }

            int GetEntityCount() { return m_entityManager->livingEntityCount; }
            
        private:
            std::unique_ptr<ECS::EntityManager> m_entityManager;
            ECS::ComponentManager* m_componentManager; 
            GameApplication* applicationInstance;
    };

}

#endif