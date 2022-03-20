#ifndef COMPONENTARRAY_H
#define COMPONENTARRAY_H

#include <stdio.h>
#include <stdint.h>
#include <vector>
#include <unordered_map>
#include "common.h"
#include "Core/logger.h"
#include "Core/assert.h"

namespace Lynx::ECS {

	class IComponentArray {
	public:
		virtual ~IComponentArray() = default;
		virtual void EntityDestroyed(uint64_t entity) = 0;
	};

	template<typename T>
	class ComponentArray : public IComponentArray {
	public:
		~ComponentArray()
		{
			components.clear();
			entityToIndexMap.clear();
			indexToEntityMap.clear();
		}

		void InsertData(EntityID entity, T component)
		{
			LYNX_ASSERT(entityToIndexMap.find(entity) == entityToIndexMap.end(), "Component added to same entity more than once.");

			size_t index = size;
			entityToIndexMap[entity] = index;
			indexToEntityMap[index] = entity;
			components.push_back(component);
			size++;
		}
		void RemoveData(EntityID entity)
		{
			LYNX_ASSERT(entityToIndexMap.find(entity) == entityToIndexMap.end(), "Component out of range");
			
			log_debug("Component remove data");
			size_t indexOfRemovedEntity = entityToIndexMap[entity];
			size_t indexOfLastElement = size - 1;
			components[indexOfRemovedEntity] = components[indexOfLastElement];
			components.pop_back(); // Pop last component since it has already been copied

			uint64_t entityOfLastElement = indexToEntityMap[indexOfLastElement];
			entityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
			indexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

			entityToIndexMap.erase(entity);
			indexToEntityMap.erase(indexOfLastElement);

			size--;
		}

		T* GetData(EntityID entity)
		{
			if(entityToIndexMap.find(entity) == entityToIndexMap.end()) {
				log_fatal("ComponentArray : Unable to find component %s in entity %ld", typeid(T).name(), entity);
				exit(0);
			}

			return &components[entityToIndexMap[entity]];
		}

		bool HasData(EntityID entity) {
			return entityToIndexMap.find(entity) != entityToIndexMap.end();
		}

		void EntityDestroyed(EntityID entity) override
		{
			log_debug("Destroying all components related to entity %d", entity);
			if (entityToIndexMap.find(entity) == entityToIndexMap.end()) {
				RemoveData(entity);
			}
		}
	private:
		std::vector<T> components;
		std::unordered_map<uint64_t, size_t> entityToIndexMap;
		std::unordered_map<size_t, uint64_t> indexToEntityMap;
		size_t size;
	};

}

#endif