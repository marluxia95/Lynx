#include "physics_world.h"
#include "Core/event_manager.h"

namespace Lynx::Physics {

    World::World(EntityManager* entityManager) : m_entity_manager(entityManager)
    {
        LYNX_ASSERT(entityManager, "Physics::World::World() : Invalid entity manager");
        EventManager::AddListener(UpdateTick, [this](const Event& ev){Update();});
        log_debug("Initialized physics world");
    }

    World::~World() = default;

    PhysEnt* World::CreateEntity()
    {
        PhysEnt* new_ent = std::static_cast<PhysEnt*>(m_entity_manager->CreateEntity());
        m_entities.push_back(new_ent);
        return new_ent;
    }

    PhysEnt* World::CreateEntity(const char* name)
    {

    }

    void World::Update()
    {
        for(auto ent : m_entities) {
            log_debug("Physical Entity ID %d", ent->GetId());
        }
    }

}
