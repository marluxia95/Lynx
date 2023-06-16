#include <stdio.h>
#include <stdlib.h>
#include "logger.h"
#include "module.h"
#include "Utils/path.hpp"

#if defined(__linux__) // || defined(__APPLE__) no apple support
	#include "impl/posix_module_impl.hpp"
#elif defined(_WIN32) || defined(_WIN64)
	#include "impl/win_module_impl.hpp"
#endif

namespace Lynx {

	std::map<std::string, IModuleLoader<IModule>*> ModuleManager::loaded_modules = std::map<std::string, IModuleLoader<IModule>*>();

	/**
	 * @brief Loads an engine module, then initializes it
	 *
	 * @param path
	 * @return std::shared_ptr<IModule>
	 */
	std::shared_ptr<IModule> ModuleManager::LoadEngineModule(std::string path)
	{
		std::string module_name = Utils::GetFileName(path);

		loaded_modules[module_name] = new ModuleLoader<IModule>(module_name);
		loaded_modules[module_name]->Load();
		loaded_modules[module_name]->GetInstance()->Init();

		return loaded_modules[module_name]->GetInstance();
	}

	void ModuleManager::UnloadAllModules()
	{
		log_debug("Unloading modules...");
		for(auto const& [n,i] : loaded_modules) {
			if(i->GetInstance()->GetName())
				log_debug("Unloading %s", i->GetInstance()->GetName());
			else
				log_debug("Unloading %s", typeid(i).name());

			i->Unload();
			delete i;
		}
		loaded_modules.clear();
	}

	std::shared_ptr<IModule> ModuleManager::GetModule(std::string path)
	{
		return loaded_modules[path]->GetInstance();
	}

}
