#include <stdio.h>
#include "componentArray.h"
#include "entity.h"

namespace Lynx {

template<typename T>
void ComponentArray<T>::InsertData(Entity entity, T Component) {
	assert(entityToIndexMap.find(entity.id) == entityToIndexMap.end() && "Component added to same entity more than once.");

	size_t index = size;
	entityToIndexMap[entity.id] = index;
	indexToEntityMap[index] = entity.id;
	size++;
}

template<typename T>
void ComponentArray<T>::RemoveData(Entity entity) {
	assert(entityToIndexMap.find(entity.id) != entityToIndexMap.end() && "Component out of range");

	size_t indexOfRemovedEntity = entityToIndexMap[entity.id];
	size_t indexOfLastElement = size - 1;
	components[indexOfRemovedEntity] = components[indexOfLastElement];

	Entity entityOfLastElement = indexToEntityMap[indexOfLastElement];
	entityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
	indexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

	entityToIndexMap.erase(entity);
	indexToEntityMap.erase(indexOfLastElement);

	size--;
}

template<typename T>
T& ComponentArray<T>::GetData(Entity entity) {
	assert(entityToIndexMap.find(entity.id) != mEntityToIndexMap.end() && "Retrieving non-existent component.");
	return components[entityToIndexMap[entity.id]];
}

template<typename T>
void ComponentArray<T>::EntityDestroyed(Entity entity) {
	if (entityToIndexMap.find(entity.id) != entityToIndexMap.end) {
		RemoveData(entity.id);
	}
}

}