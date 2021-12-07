#ifndef COMPONENT_H
#define COMPONENT_H

#include <stdio.h>
#include <stdint.h>
#include <unordered_map>
#include <memory>
#include "common.h"
#include "Core/logger.h"
#include "componentArray.h"
#include "Core/assert.h"
#include "Core/memman.h"

//#define COMPONENT_DEBUG

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
		log_debug("Registering component '%s'", typeName);
	
#ifdef COMPONENT_DEBUG
		Debug();
#endif
		//componentArrays.insert( { typeName, std::allocate_shared<ComponentArray<T>>(MemoryAllocator<ComponentArray<T>>()) } );
		componentArrays.insert( { typeName, std::make_shared<ComponentArray<T>>() } );

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
	void AddComponent(EntityID entity, T component)
	{
		GetComponentArray<T>()->InsertData(entity, component);
	}

	template<typename T>
	void RemoveComponent(EntityID entity)
	{
		GetComponentArray<T>()->RemoveData(entity);
	}

	template<typename T>
	T* GetComponent(EntityID entity)
	{
		return GetComponentArray<T>()->GetData(entity);
	}

	void EntityDestroyed(EntityID entity)
	{
		for (auto const& pair : componentArrays) {
			auto const& component = pair.second;
			component->EntityDestroyed(entity);
		}
	}

private:
	void Debug() {
		log_debug("Registered Components : ");
		int i = 0;
		for(auto [k,v] : componentTypes) {
			i++;
			log_debug(" %d : '%s' -> %x", i, k, v);
		}
	}

private:
	std::unordered_map<const char*, ComponentType> componentTypes{};
	std::unordered_map<const char*, std::shared_ptr<IComponentArray>> componentArrays{};

	ComponentType nextComponentType{};

	template<typename T>
	std::shared_ptr<ComponentArray<T>> GetComponentArray(){
		const char* typeName = typeid(T).name();

#ifdef COMPONENT_DEBUG
		Debug();
		log_debug("Getting component array '%s'", typeName );
#endif
		

		LYNX_ASSERT(componentTypes.find(typeName) != componentTypes.end(), "Component not registered before use.");

		return std::static_pointer_cast<ComponentArray<T>>(componentArrays[typeName]);
	}

};

}


#endif 
