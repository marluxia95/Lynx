#include <stdio.h>
#include "parentingSystem.h"

#include "Core/simpleGameEngine.h"

#include "Core/ECS/entity.h"
#include "Core/ECS/components.h"

extern Lynx::Application gApplication;

namespace Lynx::ECS{

    void ParentingSystem::Init()
    {

    }

    void ParentingSystem::Update()
    {
        for ( auto const ent : entities ) {
            auto transform = gApplication.GetComponent<Transform>(ent);
            auto parentEntity = gApplication.GetComponent<Parent>(ent)->parentEntity;

            // Copy the transform properties from the parent entity
            transform = gApplication.GetComponent<Transform>(parentEntity); 
        }
    }
}