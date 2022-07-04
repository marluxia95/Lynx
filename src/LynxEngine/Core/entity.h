#ifndef ENTITY_H
#define ENTITY_H

#include <glm/glm.hpp>
#include "Graphics/renderer.h"

namespace Lynx {

    class LYNXENGINE_API Entity {
    public:
        Entity(bool renderable=false) : m_isRenderable(renderable) {}
        Entity(const char* name, bool renderable=false) : m_name(name) {}
        virtual ~Entity() = default;

        const char* GetName();

        void SetName(const char* name);

        virtual void Think() {}

        glm::vec3 GetGlobalPosition() const;

        void SetGlobalPosition(glm::vec3 position);

        glm::quat GetGlobalRotation() const;

        void SetGlobalRotation(glm::quat rotation);

        glm::vec3 GetGlobalScaling() const;

        void SetGlobalScaling(glm::vec3 scale);

        /**
         * @brief Gets the entity's model matrix, for later shader
         *        processing
         * 
         * @return glm::mat4 
         */
        glm::mat4 GetModelMatrix() const;

        /**
         * @brief Gets the Render handler pointer , returns NULL if 
         *        its not meant to be rendered
         * 
         * @return RenderHndlPtr
         */
        Graphics::RenderHndlPtr GetRenderHndl() const;

        /**
         * @brief Checks if the object is renderable
         * 
         * @return true 
         * @return false 
         */
        bool IsRenderable() const;

        void MakeRenderable(bool renderable);

        std::vector<std::shared_ptr<Entity>>* GetChildren();

        void AddChild(std::shared_ptr<Entity> child);

        /**
         * @brief Sets the Renderable object
         * 
         * @param render_obj 
         */
        void SetRenderObj(Graphics::RenderHndlPtr render_obj);

    protected:
        glm::vec3 m_position, m_scale;
        glm::quat m_rotation;

        bool m_isRenderable;

        const char* m_name;    
        Graphics::RenderHndlPtr m_renderable;
        
        std::shared_ptr<Entity> m_parent;
        std::vector<std::shared_ptr<Entity>> m_children;
    };

    typedef std::shared_ptr<Entity> EntityHandlePtr;

}

#endif // ENTITY_H