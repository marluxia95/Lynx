#ifndef PHYSICS_WORLD_H
#define PHYSICS_WORLD_H

#include <glm/glm.hpp>
#include "Core/entity_manager.h"
#include "physics_ent.h"

namespace Lynx::Physics {

    /*
     * Physics::World : Manages and updates all physical entities, according to their physical properties.
     * Since physical entities are derived from normal entities, they must be managed by the EntityManager
     * to avoid possible bugs or memory leaks. Plus, it allows rendering.
     */
    class World {
        protected:
            std::vector<PhysEnt> m_entities;
            EntityManager* m_entity_manager;
            glm::vec3 m_gravity = glm::vec3(0.0f, -9.8f, 0.0f);

        public:
            World(EntityManager* manager);
            ~World();

            PhysEnt* CreateEntity();
            PhysEnt* CreateEntity(const char* name);

            void Update();
    };

}

#endif // PHYSICS_WORLD_H
