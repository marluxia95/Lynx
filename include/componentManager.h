#ifndef COMPONENT_H
#define COMPONENT_H

#include <stdio.h>
#include <stdint.h>
#include <unordered_map>
#include <memory>
#include "entity.h"
#include "componentArray.h"

namespace Lynx {

using ComponentType = uint64_t;

class ComponentManager {

public:
	template<typename T>
	void RegisterComponent();

	template<typename T>
	ComponentType GetComponentType();

	template<typename T>
	void AddComponent(Entity entity, T component);

	template<typename T>
	void RemoveComponent(Entity entity);

	template<typename T>
	T& GetComponent(Entity entity);

	void EntityDestroyed(Entity entity);

private:
	std::unordered_map<const char*, ComponentType> componentTypes{};
	std::unordered_map<const char*, std::shared_ptr<IComponentArray>> componentArrays{};

	ComponentType nextComponentType{};

	template<typename T>
	std::shared_ptr<ComponentArray<T>> GetComponentArray();

};

}


#endif COMPONENT_H
