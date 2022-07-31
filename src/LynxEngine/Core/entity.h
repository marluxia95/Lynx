#ifndef ENTITY_H
#define ENTITY_H

#include <glm/glm.hpp>
#include "Graphics/renderer.h"

namespace Lynx {

    typedef uint64_t EntityID;

    class EntityManager;

    class LYNXENGINE_API Entity {
    protected:
        glm::mat4 calcLocalModelMatrix() const;
        void updateModel();

    public:
        Entity(EntityManager* entity_manager, EntityID id, bool renderable=false);
        Entity(EntityManager* entity_manager, EntityID id, const char* name, bool renderable=false);
        ~Entity();

        const char* GetName();

        void SetName(const char* name);

        EntityID GetId();

        void Delete();

        virtual void Think() {}

        glm::vec3 GetGlobalPosition() const;

        //void SetGlobalPosition(glm::vec3 position);

        glm::vec3 GetGlobalRotation() const;

        //void SetGlobalRotation(glm::quat rotation);

        glm::vec3 GetGlobalScaling() const;

        //void SetGlobalScaling(glm::vec3 scale);

        glm::vec3 GetLocalPosition() const;

        void SetLocalPosition(glm::vec3 pos);

        glm::vec3 GetLocalRotation() const;

        void SetLocalRotation(glm::vec3 rot);

        glm::vec3 GetLocalScaling() const;

        void SetLocalScaling(glm::vec3 scale);

        /**
         * @brief Gets the entity's model matrix
         *
         * @return glm::mat4
         */
        glm::mat4 GetModelMatrix();

        /**
         * @brief Gets the Render handler pointer , returns NULL if
         *        its not meant to be rendered
         *
         * @return RenderHndlPtr
         */
        Graphics::Renderable *GetRenderHndl() const;

        /**
         * @brief Checks if the object is renderable
         *
         * @return true
         * @return false
         */
        bool IsRenderable() const;

        void MakeRenderable(bool renderable);

        std::vector<Entity*>* GetChildren();

        void AddChild(Entity* child);

        uint GetChildrenCount();

        Entity* GetChildByIndex(uint index);

        /**
         * @brief Sets the Renderable object
         *
         * @param render_obj
         */
        void SetRenderObj(Graphics::Renderable* render_obj);

        // DEBUG
        void PrintHierarchy();

    protected:
        glm::vec3 m_position = glm::vec3(0);
        glm::vec3 m_rotation;
        glm::vec3 m_scale = { 1.0f, 1.0f, 1.0f };
        glm::mat4 m_model = glm::mat4(1.0f);

        bool m_isRenderable = false;

        const char* m_name = "Unnamed";
        Graphics::Renderable* m_renderable;

        Entity* m_parent;
        std::vector<Entity*> m_children;

        EntityManager* m_entityManager = nullptr;
        EntityID m_id = 0;
    };


}

#endif // ENTITY_H
