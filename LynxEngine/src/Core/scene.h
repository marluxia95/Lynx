#ifndef SCENE_H
#define SCENE_H

#include <stdio.h>
#include <memory>

#include "entity.h"
#include "ECS/entityManager.h"
#include "ECS/componentManager.h"
#include "ECS/systemManager.h"
#include "ECS/components.h"

namespace Lynx {

    class Scene {
        public:
            Scene() {

            }

            Entity CreateEntity(const char* name) {
                Entity ent = m_entityManager->CreateEntity();
                m_componentManager->AddComponent(ent, Generic{name});
                return ent:
            }

            Entity CreateEntity() {
                Entity ent = m_entityManager->CreateEntity();
                return ent;
            }

            void DestroyEntity(Entity entity) {
                m_entityManager->DestroyEntity(ent);
            }

            template<typename T>
            void RegisterComponent(){
                componentManager->RegisterComponent<T>();
            }

            template<typename T>
            void AddComponent(Entity entity, T component){
                componentManager->AddComponent(entity, component);

                auto signature = entityManager->GetSignature(entity);
                signature.set(componentManager->GetComponentType<T>(), true);
                entityManager->SetSignature(entity, signature);

                systemManager->EntitySignatureChanged(entity, signature);
            }

            template<typename T>
            void RemoveComponent(Entity entity){
                componentManager->RemoveComponent<T>(entity);

                auto signature = entityManager->GetSignature(entity);
                signature.set(componentManager->GetComponentType<T>(), false);
                entityManager->SetSignature(entity, signature);

                systemManager->EntitySignatureChanged(entity, signature);
            }

            template<typename T>
            T* GetComponent(Entity entity){
                return m_componentManager->GetComponent<T>(entity);
            }

            template<typename T>
            ECS::ComponentType GetComponentType(){
                return m_componentManager->GetComponentType<T>();
            }

            int GetEntityCount() { return m_entityManager->livingEntityCount; }
            

        protected:
            std::unique_ptr<ECS::EntityManager> m_entityManager;
            std::unique_ptr<ECS::ComponentManager> m_componentManager;
            
            

    }

}

#endif