/**
 * PhysicsModule : A physics engine module for Lynx. Implements kinematics simulation and simple collision detection
 * to the Lynx game engine.
 * This is a prototype for now, so don't expect it to be a masterpiece
 */
#include "Core/module.h"
#include "Core/logger.h"

namespace Lynx {

class PhysicsModule : public IModule {
    public:
        PhysicsModule() : IModule("LynxPhysics") { }
        ~PhysicsModule() { Last(); }

        void Init()
        {
            log_debug("Hello from the physics engine module");
        }

        void Last()
        {
            log_debug("Goodbye!");
        }
    private:

};

}

extern "C" {
    Lynx::IModule* alloc()
    {
         return new Lynx::PhysicsModule();
    }

    void dealloc(Lynx::IModule* module)
    {
        delete module;
    }
}
