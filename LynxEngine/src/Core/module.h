#ifndef LYNX_MODULE_H
#define LYNX_MODULE_H

#include <string>
#include <map>
#include <memory>

namespace Lynx {

    class IModule {
        public:
            IModule(int id) : id(id) {}
            
            virtual ~IModule() = default;
            
            virtual void Init() = 0;
            
            virtual void Last() = 0;

            int GetID() { return id; }
        private:
            int id;
    };

    template<class T> 
    class IModuleLoader {
        public:
            ~IModuleLoader() = default;

            virtual void Load() = 0;
            
            virtual void Unload() = 0;

            virtual std::shared_ptr<T> GetInstance() = 0;

        private:
            T* module_instance;

    };

    class ModuleManager {

        public:
            static IModule* LoadEngineModule(std::string path);
            static void UnloadAllModules();

        private:
            static std::map<std::string, IModuleLoader<IModule>*> loaded_modules;
    };

}

#endif // LYNX_MODULE_H