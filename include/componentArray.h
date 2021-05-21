#ifndef COMPONENTARRAY_H
#define COMPONENTARRAY_H

#include <stdio.h>
#include <stdint.h>
#include <vector>
#include <unordered_map>
#include "entity.h"

namespace Lynx {

	class IComponentArray {
	public:
		virtual ~IComponentArray() = default;
		virtual void EntityDestroyed(uint64_t entity) = 0;
	};

	template<typename T>
	class ComponentArray : public IComponentArray {
	public:
		void InsertData(Entity entity, T Component){
			assert(entityToIndexMap.find(entity) != entityToIndexMap.end() && "Component added to same entity more than once.");

			size_t index = size;
			entityToIndexMap[entity] = index;
			indexToEntityMap[index] = entity;
			size++;
		}
		void RemoveData(Entity entity){
			assert(entityToIndexMap.find(entity) != entityToIndexMap.end() && "Component out of range");

			size_t indexOfRemovedEntity = entityToIndexMap[entity];
			size_t indexOfLastElement = size - 1;
			components[indexOfRemovedEntity] = components[indexOfLastElement];

			uint64_t entityOfLastElement = indexToEntityMap[indexOfLastElement];
			entityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
			indexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

			entityToIndexMap.erase(entity);
			indexToEntityMap.erase(indexOfLastElement);

			size--;
		}
		T& GetData(Entity entity){
			return components[entityToIndexMap[entity]];
		}
		void EntityDestroyed(Entity entity) override{
			if (entityToIndexMap.find(entity) != entityToIndexMap.end()) {
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