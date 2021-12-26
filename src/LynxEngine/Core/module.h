#ifndef LYNX_MODULE_H
#define LYNX_MODULE_H

#include <string>
#include <map>
#include <memory>
#include "lynx_common.h"

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

    };

    class LYNXENGINE_API ModuleManager {

        public:
            static std::shared_ptr<IModule> LoadEngineModule(std::string path);
            static void UnloadAllModules();
            static std::shared_ptr<IModule> GetModule(std::string path);

        private:
            static std::map<std::string, IModuleLoader<IModule>*> loaded_modules;
    };

}

#endif // LYNX_MODULE_H