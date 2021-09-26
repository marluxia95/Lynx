#include <stdio.h>
#include "parentingSystem.h"

#include "Core/application.h"


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
            auto transform = scene->GetComponent<Transform>(ent);
            auto parentEntity = scene->GetComponent<Parent>(ent)->parentEntity;

            // Copy the transform properties from the parent entity
            transform = scene->GetComponent<Transform>(parentEntity); 
        }
    }
}