/**
 * @name entity.h
 * @author marlxx
 * @brief Front-end interface for ECS entities. Meant to be used by user, instead of IDs
*/ 
#ifndef ENTITY_H
#define ENTITY_H

#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include "Core/scene.h"
#include "common.h"

namespace Lynx {

    class scene;

    /*
        Front-end class for ECS API 
    */
    class LYNXENGINE_API Entity {
    public:
        Entity() : m_id(-1) {}
        Entity(const EntityID& id);
        Entity(Scene* scene, EntityID id);

        operator EntityID() const { return m_id; }

        template<class T>
        T* GetComponent()
        {
            return m_scene->GetComponent<T>(m_id);
        }

        template<class T>
        void RemoveComponent()
        {
            m_scene->RemoveComponent<T>(m_id);
        }

        template<class T>
        void AddComponent(T component)
        {
            m_scene->AddComponent(m_id, component);
        }

        template<class T>
        bool HasComponent()
        {
            return m_scene->HasComponent<T>(m_id);
        }

        void AddChild(Entity ent);

        bool IsValid() { return m_id > 0; }

        std::vector<EntityID> GetChildren();

        glm::vec3 GetLocalPosition();
        glm::vec3 GetWorldPosition();

    private:
        std::shared_ptr<Scene> m_scene;
        EntityID m_id;
        EntityID m_parentEnt;
    };

}

#endif 