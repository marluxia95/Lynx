#ifndef POSIX_LYNX_MODULE_H
#define POSIX_LYNX_MODULE_H

#if defined(__linux__) || defined(__APPLE__)

#include <stdio.h>
#include <string>
#include <dlfcn.h>
#include <memory>
#include "Core/logger.h"
#include "Core/module.h"

namespace Lynx {

    template<class T> 
    class ModuleLoader : public IModuleLoader<T> {
        public:
            ModuleLoader(const std::string& path, 
                        const std::string& alloc_sym = "alloc", 
                        const std::string& dealloc_sym = "dealloc") : 
                        m_path(path), m_allocSymbol(alloc_sym), m_deallocSymbol(dealloc_sym) {
                

            }

            ~ModuleLoader() {
                Unload();
            }

            void Load() {
                if(!(m_handle = dlopen(m_path.c_str(), RTLD_LAZY | RTLD_NOW))) {
                    log_error("Couldn't open module %s ! %s", m_path.c_str(), dlerror());
                }
            }
            
            void Unload() {
                if(dlclose(m_handle) != 0) {
                    log_error("Couldn't close module %s ! %s", m_path.c_str(), dlerror());
                }
            }

            std::shared_ptr<T> GetInstance() {
                auto alloc_fn   = reinterpret_cast<T*(*)()>(dlsym(m_handle, m_allocSymbol.c_str()));
                auto dealloc_fn = reinterpret_cast<void(*)(T*)>(dlsym(m_handle, m_deallocSymbol.c_str()));

                if(!alloc_fn | !dealloc_fn) {
                    log_error("Unable to load symbol from module %s ! %s", m_path, dlerror());
                }

                return std::shared_ptr<T>( alloc_fn(), [dealloc_fn](T* i) { dealloc_fn(i); } );
            }

        private:
            std::string m_path;
            std::string m_allocSymbol;
            std::string m_deallocSymbol;
            void* m_handle;

    };
    
}

#endif // __linux__
#endif // POSIX_LYNX_MODULE_H