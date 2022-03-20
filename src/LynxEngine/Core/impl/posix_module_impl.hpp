#ifndef POSIX_LYNX_MODULE_H
#define POSIX_LYNX_MODULE_H

#if defined(__linux__) || defined(__APPLE__)

#include <stdio.h>
#include <string>
#include <dlfcn.h>
#include <memory>
#include "Core/logger.h"
#include "Core/module.h"
#include "Utils/path.hpp"

namespace Lynx {

    template<class T> 
    class ModuleLoader : public IModuleLoader<T> {
        public:
            ModuleLoader(const std::string& path, 
                        const std::string& alloc_sym = "alloc", 
                        const std::string& dealloc_sym = "dealloc") : 
                        m_path(Utils::GetPosixLibraryPath(path)), m_allocSymbol(alloc_sym), m_deallocSymbol(dealloc_sym) {
                

            }

            void Load() {
                if(!(m_handle = dlopen(m_path.c_str(), RTLD_NOW))) {
                    log_error("Couldn't open module %s ! %s", m_path.c_str(), dlerror());
                    return;
                }

                log_debug("Successfully loaded module %s", m_path.c_str());
            }
            
            void Unload() {
                m_moduleInstance->Last();
                if(dlclose(m_handle) != 0) {
                    log_error("Couldn't close module %s ! %s", m_path.c_str(), dlerror());
                    return;
                }
            }

            std::shared_ptr<T> GetInstance() {
                if(!m_handle){return NULL;}
                if(m_moduleInstance) { return m_moduleInstance; }

                auto alloc_fn   = reinterpret_cast<T*(*)()>(dlsym(m_handle, m_allocSymbol.c_str()));
                auto dealloc_fn = reinterpret_cast<void(*)(T*)>(dlsym(m_handle, m_deallocSymbol.c_str()));

                if(!alloc_fn | !dealloc_fn) {
                    log_error("Unable to load symbol from module %s ! %s", m_path, dlerror());
                }

                m_moduleInstance = std::shared_ptr<T>( alloc_fn(), [dealloc_fn](T* i) { dealloc_fn(i); } );

                return m_moduleInstance;
            }

        private:
            std::string m_path;
            std::string m_allocSymbol;
            std::string m_deallocSymbol;
            void* m_handle;

            std::shared_ptr<T> m_moduleInstance;

    };
    
}

#endif // __linux__
#endif // POSIX_LYNX_MODULE_H