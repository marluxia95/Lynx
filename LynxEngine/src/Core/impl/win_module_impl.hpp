#ifndef WIN_LYNX_MODULE_H
#define WIN_LYNX_MODULE_H

// TODO
#ifdef _WIN32 

#include <stdio.h>
#include <string>

namespace Lynx {

    template<class T> class IModuleLoader {
        public:
            ModuleLoader(std::string path) = default;
            ~ModuleLoader() = default;

            virtual void Load() = 0;
            
            virtual void Unload() = 0;

        private:
            T* module_instance;

    };
    
}

#endif // _WIN32
#endif // WIN_LYNX_MODULE_H