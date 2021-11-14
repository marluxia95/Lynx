#ifndef COMPONENT_H
#define COMPONENT_H

#include <stdio.h>
#include <stdint.h>
#include <unordered_map>
#include <memory>
#include "entity.h"
#include "Core/logger.h"
#include "componentArray.h"
#include "Core/assert.h"
#include "Core/memman.h"

namespace Lynx::ECS {

using ComponentType = uint64_t;

class ComponentManager {

public:
	~ComponentManager()
	{
		componentArrays.clear();
		componentTypes.clear();
	}

	template<typename T>
	void RegisterComponent()
	{
		const char* typeName = typeid(T).name();

		LYNX_ASSERT(componentTypes.find(typeName) == componentTypes.end(), "Component Type already exists.");
		componentTypes.insert( { typeName, nextComponentType } );
		log_debug("Registering component %s ", typeName);

		componentArrays.insert( { typeName, std::allocate_shared<ComponentArray<T>>(MemoryAllocator<ComponentArray<T>>()) } );

		nextComponentType++;
	}

	template<typename T>
	ComponentType GetComponentType()
	{
		const char* typeName = typeid(T).name();

		LYNX_ASSERT(componentTypes.find(typeName) != componentTypes.end(), "Component does not exist.");

		return componentTypes[typeName];
	}

	template<typename T>
	void AddComponent(Entity entity, T component)
	{
		GetComponentArray<T>()->InsertData(entity, component);
	}

	template<typename T>
	void RemoveComponent(Entity entity)
	{
		GetComponentArray<T>()->RemoveData(entity);
	}

	template<typename T>
	T* GetComponent(Entity entity)
	{
		return GetComponentArray<T>()->GetData(entity);
	}

	void EntityDestroyed(Entity entity)
	{
		for (auto const& pair : componentArrays) {
			auto const& component = pair.second;
			component->EntityDestroyed(entity);
		}
	}

private:
	std::unordered_map<const char*, ComponentType> componentTypes{};
	std::unordered_map<const char*, std::shared_ptr<IComponentArray>> componentArrays{};

	ComponentType nextComponentType{};

	template<typename T>
	std::shared_ptr<ComponentArray<T>> GetComponentArray(){
		const char* typeName = typeid(T).name();

		LYNX_ASSERT(componentTypes.find(typeName) != componentTypes.end(), "Component not registered before use.");

		return std::static_pointer_cast<ComponentArray<T>>(componentArrays[typeName]);
	}

};

}


#endif 
