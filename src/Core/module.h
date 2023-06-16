/**
 * @file module.h
 * @author marlxx (marlxx@protonmail.com)
 * @brief Modules are external parts of the engine that are optional to use. They are basically external shared libraries that are loaded in runtime and used
 * for things such as physics, etc. This file contains all the necesary code to load, initialize and unload modules, for per-system implementations, please go to
 * impl/
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef LYNX_MODULE_H
#define LYNX_MODULE_H

#include <string>
#include <map>
#include <memory>
#include "lynx_common.h"

namespace Lynx {

	class IModule {
		public:
			IModule(const char* name) : m_name(name) {}

			virtual ~IModule() = default;

			virtual void Init() = 0;

			virtual void Last() = 0;

			const char* GetName() { return m_name; }

			int GetID() { return m_id; }
		private:
			int m_id = 0;
			const char* m_name = "";
	};

	template<class T>
	class IModuleLoader {
		public:
			~IModuleLoader() = default;

			virtual void Load() = 0;

			virtual void Unload() = 0;

			virtual std::shared_ptr<T> GetInstance() = 0;

	};

	/**
	 * @brief Class to handle module managing
	 *
	 */
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
