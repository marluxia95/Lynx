#include "Core/application.h"
#include "Core/logger.h"
#include "Core/eventManager.h"
#include "Core/module.h"
#include "physicsSystem.h"
#include "physicsObject.h"


using namespace Lynx;

class PhysicsModule : public IModule {
    public:
        PhysicsModule() : IModule(0) {}
        ~PhysicsModule() { Last(); }
                
        void Init() {
            log_debug("Loaded physics module!");
        }
                
        void Last() {
            log_debug("Unloaded physics module!");
        }
};

extern "C" { 

    IModule* alloc() 
    {
        return new PhysicsModule();
    }

    void dealloc(IModule* mod) 
    {
        delete mod;
    }

}
