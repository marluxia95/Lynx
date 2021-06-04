#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H

#include <iostream>
#include <memory>
#include <unordered_map>
#include "entity.h"
#include "system.h"

namespace Lynx::ECS {
	class SystemManager {
	public:
		template<class T>
		std::shared_ptr<T> RegisterSystem(){
			const char* typeName = typeid(T).name();

			assert(systems.find(typeName) == systems.end() && "System already exists");

			std::shared_ptr<System> system = std::dynamic_pointer_cast<System>(std::make_shared<T>());

			systems.insert({ typeName, system });
			return std::make_shared<T>();
		}	

		template<typename T>
		void SetSignature(Signature signature){
			const char* typeName = typeid(T).name();

			assert(systems.find(typeName) != systems.end() && "System used before registered" );

			signatures.insert({ typeName, signature });
		}

		void EntityDestroyed(Entity entity){
			for (auto const& pair : systems) {
				auto const& system = pair.second;
				system->entities.erase(entity);
			}
		}
		void EntitySignatureChanged(Entity entity, Signature signature){
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
	private:
		std::unordered_map<const char*, Signature> signatures{};
		std::unordered_map<const char*, std::shared_ptr<System>> systems{};
	};
}

#endif