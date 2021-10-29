#ifndef LYNX_MODULE_H
#define LYNX_MODULE_H

#include <string>
#include <vector>

namespace Lynx {

    class Module {
        public:
            Module(int id, const char* path, void* l_obj);
            ~Module();
            int GetID() { return id; }
            const char* GetPath() { return path; }
            void* GetLibraryObj() { return library_obj; }

        private:
            int id;
            const char* path;
            void* library_obj;
    };

    class ModuleLoader {

        public:
            static void UnloadModules();
            static Module* LoadModule(std::string path);

        private:
            static std::vector<Module*> loaded_modules;
    };

}

#endif // LYNX_MODULE_H