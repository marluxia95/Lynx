#ifndef WIN_LYNX_MODULE_H
#define WIN_LYNX_MODULE_H

// TODO
#ifdef _WIN32 

#include <windows.h> 
#include <stdio.h>
#include <string>
#include "Core/logger.h"

typedef int(__cdecl* m_func)();

std::string GetErrorStr()
{
    DWORD errorID = GetLastError();

    if (errorID == 0)
        return std::string();

    LPSTR msg_buff = nullptr;
    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, errorID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&msg_buff, 0, NULL);

    std::string error_msg(msg_buff, size);

    LocalFree(msg_buff);

    return error_msg;
}

namespace Lynx {

    template<class T> 
    class ModuleLoader : public IModuleLoader<T> {
    public:
        ModuleLoader(const std::string& path,
            const std::string& alloc_sym = "alloc",
            const std::string& dealloc_sym = "dealloc") :
            m_path(Utils::GetWindowsLibraryPath(path)), m_allocSymbol(alloc_sym), m_deallocSymbol(dealloc_sym) {


        }

        void Load() {
            m_hinst = LoadLibrary(TEXT(m_path.c_str()));
            if (m_hinst == NULL) {
                log_error("Unable to load module %s ! Error %x ", m_path, GetErrorStr().c_str());
            }
            log_debug("Successfully loaded library %s", Utils::GetFileName(m_path).c_str());
        }
            
        void Unload() {
            if (!m_hinst) return;

            bool free_result = FreeLibrary(m_hinst);
            if (!free_result) {
                log_error("Unable to free module %s ! Error %x ", m_path, GetErrorStr().c_str());
            }
        }

        std::shared_ptr<T> GetInstance() {
            if (!m_hinst) return NULL;
            if (m_moduleInstance) { return m_moduleInstance; }

            auto alloc_fn = reinterpret_cast<T * (*)()>((m_func)GetProcAddress(m_hinst, TEXT(m_allocSymbol.c_str())) );
            auto dealloc_fn = reinterpret_cast<void (*)(T*)>((m_func)GetProcAddress(m_hinst, TEXT(m_deallocSymbol.c_str())) );

            if (!alloc_fn | !dealloc_fn) {
                log_error("Unable to load symbol from module %s ! %s", m_path, GetErrorStr().c_str());
                return NULL;
            }

            m_moduleInstance = std::shared_ptr<T>(alloc_fn(),
                [dealloc_fn](T* i) { dealloc_fn(i); }
            );

            return m_moduleInstance;
        }

    private:
        std::string m_path;
        std::string m_allocSymbol;
        std::string m_deallocSymbol;

        HINSTANCE m_hinst;

        std::shared_ptr<T> m_moduleInstance;


    };
    
}

#endif // _WIN32
#endif // WIN_LYNX_MODULE_H