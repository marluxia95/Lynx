#include "Core/application.h"
#include "Core/logger.h"
#include "Core/moduleInterface.h"
#include "Core/eventManager.h"
#include "physicsSystem.h"
#include "physicsObject.h"

extern "C" { ENGINE_MODULE_INIT; ENGINE_MODULE_CLOSE; }

ENGINE_MODULE_INIT {
    
    Lynx::EventManager::AddListener(Lynx::FirstTick, [](const Lynx::Event&) -> int {
        Lynx::GameApplication* applicationInstance = Lynx::GameApplication::GetGameInstance();

        applicationInstance->RegisterComponent<Lynx::Physics::PhysicsObject>();

        applicationInstance->RegisterSystem<Lynx::Physics::System>();
        {
            // Signature configuration
            Signature signature;
            signature.set(applicationInstance->GetComponentType<Lynx::Physics::PhysicsObject>());
            applicationInstance->SetSystemSignature<Lynx::Physics::System>(signature);
        }

        log_debug("Added Physics system");
    });
    
    log_info("LynxPhysics module initialized");
}

ENGINE_MODULE_CLOSE {
    log_info("Module test unloaded!");
}
