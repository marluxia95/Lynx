#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H

#include <iostream>
#include <memory>
#include <unordered_map>
#include "ECS/entity.h"
#include "ECS/system.h"
#include "Core/logger.h"

namespace Lynx::ECS {
	class SystemManager {
	public:
		template<class T>
		std::shared_ptr<T> RegisterSystem(){
			const char* typeName = typeid(T).name();
			log_debug("Registering system %s ", typeName);
			assert(systems.find(typeName) == systems.end() && "System already exists");

			auto system = std::make_shared<T>();
			systems.insert({ typeName, system });
			return system;
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
			log_debug("Entity %d changed signature ! Updating systems ...", entity);
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

		void InitSystems() {
			log_info("Initializing systems...");
			for (auto const& system : systems) {
				system.second->Init();
				log_debug("System %s initialized", system.first);
			}
		}

		void UpdateSystems() {
			for (auto const& system : systems) {
				system.second->Update();
			}
		}
	private:
		std::unordered_map<const char*, Signature> signatures{};
		std::unordered_map<const char*, std::shared_ptr<System>> systems{};
	};
}

#endif