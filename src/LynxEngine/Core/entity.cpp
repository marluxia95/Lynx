#include <glm/gtx/string_cast.hpp>
#include "entity.h"
#include "logger.h"

namespace Lynx {

    const char* Entity::GetName()
    {
        return m_name;
    }

    void Entity::SetName(const char* name)
    {
        m_name = name;
    }

    glm::vec3 Entity::GetLocalPosition() const
    {
        return m_position;
    }

    void Entity::SetLocalPosition(glm::vec3 pos)
    {
        m_position = pos;
    }

    glm::vec3 Entity::GetLocalRotation() const
    {
        return m_rotation;
    }

    void Entity::SetLocalRotation(glm::vec3 rot)
    {
        m_rotation = rot;
    }

    glm::vec3 Entity::GetLocalScaling() const
    {
        return m_scale;
    }

    void Entity::SetLocalScaling(glm::vec3 scale)
    {
        m_scale = scale;
    }

    // FIXME : Add matrices and local position support
    glm::vec3 Entity::GetGlobalPosition() const 
    {
        if (m_parent)
            return GetLocalPosition() + m_parent->GetLocalPosition();

        return GetLocalPosition();
    }

    glm::quat Entity::GetGlobalRotation() const
    {
        return m_rotation;
    }

    glm::vec3 Entity::GetGlobalScaling() const
    {
        return m_scale;
    }

    glm::mat4 Entity::GetModelMatrix()
    {
        updateModel();
        return m_model;
    }

    void Entity::updateModel()
    {
        if (m_parent)
            m_model = m_parent->m_model * calcLocalModelMatrix();
        else
            m_model = calcLocalModelMatrix();

        for (Entity* child : m_children) 
        {
            child->updateModel();
        }
    }

    glm::mat4 Entity::calcLocalModelMatrix() const
    {
        glm::mat4 model = glm::mat4(1.0f);

        glm::mat4 positionMatrix = glm::translate(model, GetGlobalPosition());
        glm::mat4 scaleMatrix = glm::scale(model, m_scale);
        glm::mat4 rotationMatrix = glm::rotate(model, glm::radians(m_rotation.x), vec3(1.0f, 0.0f, 0.0f)) *
            glm::rotate(model, glm::radians(m_rotation.y), vec3(0.0f, 1.0f, 0.0f)) *
            glm::rotate(model, glm::radians(m_rotation.z), vec3(0.0f, 0.0f, 1.0f));
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
        MakeRenderable(true);
        m_renderable = render_obj;
    }

    std::vector<Entity*>* Entity::GetChildren()
    {
        return &m_children;
    }

    void Entity::AddChild(Entity* child)
    {
        if (!child)
            return;

        child->m_parent = this;
        m_children.push_back(child);
    }

    uint Entity::GetChildrenCount()
    {
        return m_children.size();
    }

    Entity* Entity::GetChildByIndex(uint index)
    {
        if (index > m_children.size())
            return NULL;
        
        return m_children[index];
    }

    // shitty
    void Entity::PrintHierarchy()
    {
        log_debug("+ E %d %s", m_id, m_name);

        if(!m_children.size())
            return;

        log_debug("|");
        
        for(auto c : m_children) 
        {
            c->PrintHierarchy();
        }
        
    }

}