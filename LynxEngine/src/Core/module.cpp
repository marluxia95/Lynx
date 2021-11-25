#include <stdio.h>
#include <stdlib.h>
#include "logger.h"
#include "module.h"

#if defined(__linux__) || defined(__APPLE__)
    #include "impl/posix_module_impl.hpp"
#elif defined(_WIN32) || defined(_WIN64)
    #include "impl/win_module_impl.hpp"
#endif

namespace Lynx {
    
    std::map<std::string, IModuleLoader<IModule>*> ModuleManager::loaded_modules = std::map<std::string, IModuleLoader<IModule>*>();

    IModule* ModuleManager::LoadEngineModule(std::string path)
    {
#if defined(__linux__) || defined(__APPLE__)
        loaded_modules[path] = new ModuleLoader<IModule>(path);
#elif defined(_WIN32) || defined(_WIN64)

#endif
    }

    void ModuleManager::UnloadAllModules()
    {

    }

}