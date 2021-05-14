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
		virtual void EntityDestroyed(Entity entity) = 0;
	};

	template<typename T>
	class ComponentArray : public IComponentArray {
	public:
		void InsertData(Entity entity, T Component);
		void RemoveData(Entity entity);
		T& GetData(Entity entity);
		void EntityDestroyed(Entity entity) override;
	private:
		std::vector<T> components;
		std::unordered_map<uint64_t, size_t> entityToIndexMap;
		std::unordered_map<size_t, uint64_t> indexToEntityMap;
		size_t size;
	};

}

#endif