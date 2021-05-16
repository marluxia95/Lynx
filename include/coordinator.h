#ifndef COORDINATOR_H
#define COORDINATOR_H

#include <stdio.h>
#include <memory>
#include "entityManager.h"
#include "componentManager.h"
#include "systemManager.h"
#include "entity.h"

namespace Lynx {

	class Coordinator {
	public:
		void Init();

		Entity CreateEntity();

		void DestroyEntity(Entity entity);

		template<typename T>
		void RegisterComponent();
		
		template<typename T>
		void AddComponent(Entity entity, T component);

		template<typename T>
		void RemoveComponent(Entity entity);

		template<typename T>
		T& GetComponent(Entity entity);

		template<typename T>
		ComponentType GetComponentType();

		template<typename T>
		std::shared_ptr<T> RegisterSystem();

		template<typename T>
		void SetSystemSignature(Signature signature);
	
	private:
		std::unique_ptr<ComponentManager> componentManager;
		std::unique_ptr<EntityManager> entityManager;
		std::unique_ptr<SystemManager> systemManager;
	};

}


#endif 