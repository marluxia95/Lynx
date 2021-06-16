#include <stdio.h>
#include "parentingSystem.h"
#include "simpleGameEngine.h"
#include "entity.h"
#include "components.h"

extern Lynx::Core::Game game;

namespace Lynx::ECS{

    void ParentingSystem::Init()
    {

    }

    void ParentingSystem::Update()
    {
        for ( auto const ent : entities ) {
            auto transform = game.GetComponent<Transform>(ent);
            auto parentEntity = game.GetComponent<Parent>(ent)->parentEntity;

            // Copy the transform properties from the parent entity
            transform = game.GetComponent<Transform>(parentEntity); 
        }
    }
}