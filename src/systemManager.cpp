#include <stdio.h>
#include <assert.h>
#include <memory>
#include "systemManager.h"

namespace Lynx {
	template<typename T>
	std::shared_ptr<T> SystemManager::RegisterSystem(){
		const char* typeName = typeid(T).name();

		assert(systems.find(typeName) == systems.end() && "System already exists");

		auto system = std::make_shared<T>();
		system.insert({ typeName, system });
		return system;
	}

	template<typename T>
	void SystemManager::SetSignature(Signature signature) {
		const char* typeName = typeid(T).name();

		assert(systems.find(typeName) != systems.end() && "System used before registered" );

		signatures.insert({ typeName, signature });
	}

	void SystemManager::EntityDestroyed(Entity entity) {
		for (auto const& pair : systems) {
			auto const& system = pair.second;
			system->entities.erase(entity);
		}
	}

	void SystemManager::EntitySignatureChanged(Entity entity, Signature signature) {
		// Notify each system that an entity's signature changed
		for (auto const& pair : systems)
		{
			auto const& type = pair.first;
			auto const& system = pair.second;
			auto const& systemSignature = signatures[type];

			// Entity signature matches system signature - insert into set
			if ((signature & systemSignature) == systemSignature)
			{
				system->entities.insert(entity);
			}
			// Entity signature does not match system signature - erase from set
			else
			{
				system->entities.erase(entity);
			}
		}
	}
}