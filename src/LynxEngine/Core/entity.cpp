#include "entity.h"

namespace Lynx {

    const char* Entity::GetName()
    {
        return m_name;
    }

    void Entity::SetName(const char* name)
    {
        m_name = name;
    }

    // FIXME : Add matrices and local position support
    glm::vec3 Entity::GetGlobalPosition() const 
    {
        return m_position;
    }

    void Entity::SetGlobalPosition(glm::vec3 position)
    {
        m_position = position;
    }

    glm::quat Entity::GetGlobalRotation() const
    {
        return m_rotation;
    }

    void Entity::SetGlobalRotation(glm::quat rotation)
    {
        m_rotation = rotation;
    }   

    glm::vec3 Entity::GetGlobalScaling() const
    {
        return m_scale;
    }

    void Entity::SetGlobalScaling(glm::vec3 scale)
    {
        m_scale = scale;
    }

    glm::mat4 Entity::GetModelMatrix() const
    {
        glm::mat4 model = glm::mat4(1.0f);

        glm::mat4 positionMatrix = glm::translate(model, m_position);
        glm::mat4 scaleMatrix = glm::scale(model, m_scale);
        glm::mat4 rotationMatrix = mat4_cast(m_rotation);

        model = positionMatrix * scaleMatrix * rotationMatrix;

        return model;
    }

    Graphics::Renderable* Entity::GetRenderHndl() const
    {
        return m_renderable;
    }

    bool Entity::IsRenderable() const
    {
        return m_isRenderable;
    }

    void Entity::MakeRenderable(bool renderable)
    {
        m_isRenderable = renderable;
    }

    void Entity::SetRenderObj(Graphics::Renderable* render_obj)
    {
        m_renderable = render_obj;
    }

    std::vector<Entity*>* Entity::GetChildren()
    {
        return &m_children;
    }

    void Entity::AddChild(Entity* child)
    {
        child->m_parent = this;
        m_children.push_back(child);
    }

}