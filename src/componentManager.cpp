#include <stdio.h>
#include "entity.h"
#include "componentManager.h"

namespace Lynx {
	template<typename T>
	void ComponentManager::RegisterComponent() {
		const char* typeName = typeid(T).name();

		assert(componentTypes.find(typeName) == componentTypes.end() && "Component Type already exists.");
		componentTypes.insert({ typeName, nextComponentType });

		componentArrays.insert({ typeName, std::make_shared<ComponentArray<T>> });

		nextComponentType++;
	}

	template<typename T>
	ComponentType ComponentManager::GetComponentType() {
		const char* typenName = typeid(T).name();

		assert(componentTypes.find(typeName) != componentTypes.end() && "Component does not exist.");

		return componentTypes[typeName];
	}

	template<typename T>
	void ComponentManager::AddComponent(Entity entity, T component) {
		GetComponentArray<T>()->InsertData(entity, component);
	}

	template<typename T>
	void ComponentManager::RemoveComponent(Entity entity) {
		GetComponentArray<T>()->RemoveData(entity);
	}

	template<typename T>
	T& ComponentManager::GetComponent(Entity entity) {
		return GetComponentArray<T>()->GetData(entity);
	}

	void ComponentManager::EntityDestroyed(Entity entity) {
		for (auto const& pair : componentArrays) {
			auto const& component = pair.second;
			component->EntityDestroyed(entity);
		}
	}
}



