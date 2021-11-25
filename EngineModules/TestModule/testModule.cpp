#include "Core/logger.h"

#include "Core/module.h"

using namespace Lynx;

class TestModule : public IModule {
    public:
        TestModule() : IModule(0) {}
        ~TestModule() { Last(); }
            
        void Init() {
            log_debug("Hello!");
        }
            
        void Last() {
            log_debug("Goodbye!");
        }

};

extern "C" { 

    IModule* alloc() 
    {
        return new TestModule();
    }

    void dealloc(IModule* mod) 
    {
        delete mod;
    }

}
