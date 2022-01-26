#include "Core/application.h"
#include "entity.h"

namespace Lynx {

    /*
        TODO
    */

    Entity::Entity(const EntityID& id) : m_id(id), m_scene(GameApplication::GetGameInstance()->GetScene())
    {   
        AddComponent(Children{});
    }

    Entity::Entity(Scene* scene, EntityID id) : m_id(id), m_scene(scene) 
    {
        AddComponent(Children{});
    }

    void Entity::AddChild(Entity ent) 
    {
        if(!HasComponent<Children>())
            AddComponent(Children{});

        GetComponent<Children>()->entityChildren.push_back(ent);
    }

    std::vector<EntityID> Entity::GetChildren() 
    {
        if(!HasComponent<Children>())
            AddComponent(Children{});

        return GetComponent<Children>()->entityChildren;
    }

    glm::vec3 Entity::GetLocalPosition()
    {
        return glm::vec3();
    }

    glm::vec3 Entity::GetWorldPosition()
    {
        return glm::vec3();
    }

}