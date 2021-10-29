#include <stdio.h>
#include <stdlib.h>
#ifdef __linux__
#include <dlfcn.h>
#endif
#include "logger.h"
#include "module.h"

namespace Lynx {
    
    std::vector<Module*> ModuleLoader::loaded_modules = std::vector<Module*>();

    void ModuleLoader::UnloadModules()
    {
        for(int m = 0; m < loaded_modules.size(); m++) {
            delete loaded_modules[m];
        }
    }

#ifdef __linux__

    Module::Module(int id, const char* path, void* l_obj) : id(id), path(path), library_obj(l_obj) 
    {
        const char* error;

        int (*func)(void);

  
        *(void**)(&func) = dlsym(library_obj, "module_init");

        if(!func){
            log_fatal("Error in module %s! %s", path, dlerror());
            exit(EXIT_FAILURE);
        }

        dlerror();  

        func();

        if ((error = dlerror()) != NULL)  {
            log_fatal("Error in module %s : %s", path, error);
            exit(EXIT_FAILURE);
        }
    }

    Module::~Module()
    {
        log_debug("Closing module object %s", path);

        int (*func)(void);

        *(void**)(&func) = dlsym(library_obj, "module_close");

        if(!func){
            log_fatal("Error in module %s! %s", path, dlerror());
            exit(EXIT_FAILURE);
        }

        dlerror();  

        func();
        dlclose(library_obj);
    }

    Module* ModuleLoader::LoadModule(std::string path)
    {
        path = "lib" + path + ".so";
        log_debug("Loading Module %s", path.c_str());
        Module* n_mod = new Module(loaded_modules.size(), path.c_str(), dlopen(path.c_str(), RTLD_NOW));
        loaded_modules.push_back(n_mod);
    }

    

#else if _WIN32

    Module ModuleLoader::LoadModule(const char* path)
    {
        
    }

#endif

}