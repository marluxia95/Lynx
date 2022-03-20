#ifndef SCENE_H
#define SCENE_H

#include <stdio.h>
#include <memory>
#include "ECS/entityManager.h"
#include "ECS/componentManager.h"
#include "ECS/components.h"
#include "ECS/common.h"
#include "lynx_common.h"

namespace Lynx {

    class LYNXENGINE_API Scene {
        private:
            void updateSignature(EntityID entity, Signature signature);

        public:
            Scene();

            EntityID CreateEntity(const char* name);
            EntityID CreateEntity();

            void DestroyEntity(EntityID entity);

            virtual void Init() {}
            virtual void Destroy() {}

            template<typename T>
            void RegisterComponent(){
                m_componentManager->RegisterComponent<T>();
            }

            template<typename T>
            void AddComponent(EntityID entity, T component){
                m_componentManager->AddComponent(entity, component);

                auto signature = m_entityManager->GetSignature(entity);
                signature.set(m_componentManager->GetComponentType<T>(), true);
                updateSignature(entity, signature);
            }

            template<typename T>
            void RemoveComponent(EntityID entity){
                m_componentManager->RemoveComponent<T>(entity);

                auto signature = m_entityManager->GetSignature(entity);
                signature.set(m_componentManager->GetComponentType<T>(), false);
                updateSignature(entity, signature);
            }

            template<typename T>
            bool HasComponent(EntityID entity) {
                return m_componentManager->HasComponent<T>(entity);
            }

            template<typename T>
            T* GetComponent(EntityID entity){
                return m_componentManager->GetComponent<T>(entity);
            }

            int GetEntityCount() { return m_entityManager->livingEntityCount; }
            
        protected:
            std::unique_ptr<ECS::EntityManager> m_entityManager;
            std::shared_ptr<ECS::ComponentManager> m_componentManager; 
    };

}

#endif