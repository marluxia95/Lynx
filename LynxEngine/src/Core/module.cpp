#include <stdio.h>
#include <stdlib.h>
#include "logger.h"
#include "module.h"
#include "Utils/path.hpp"

#if defined(__linux__) || defined(__APPLE__)
    #include "impl/posix_module_impl.hpp"
#elif defined(_WIN32) || defined(_WIN64)
    #include "impl/win_module_impl.hpp"
#endif

namespace Lynx {
    
    std::map<std::string, IModuleLoader<IModule>*> ModuleManager::loaded_modules = std::map<std::string, IModuleLoader<IModule>*>();

    std::shared_ptr<IModule> ModuleManager::LoadEngineModule(std::string path)
    {
        std::string module_name = Utils::GetFileName(path);

        loaded_modules[module_name] = new ModuleLoader<IModule>(module_name);
        loaded_modules[module_name]->Load();

        return loaded_modules[module_name]->GetInstance();
    }

    void ModuleManager::UnloadAllModules()
    {
        log_debug("Unloading modules...");
        for(auto const& [n,i] : loaded_modules) {
            i->Unload();
            delete i;
        }

    }

    std::shared_ptr<IModule> ModuleManager::GetModule(std::string path)
    {
        return loaded_modules[path]->GetInstance();
    }

}