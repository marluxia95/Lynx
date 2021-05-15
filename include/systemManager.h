#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H

#include <memory>
#include <unordered_map>
#include "entity.h"
#include "system.h"

namespace Lynx {
	class SystemManager {
	public:
		template<typename T>
		std::shared_ptr<T> RegisterSystem();

		template<typename T>
		void SetSignature(Signature signature);

		void EntityDestroyed(Entity entity);
		void EntitySignatureChanged(Entity entity, Signature signature);
	private:
		std::unordered_map<const char*, Signature> signatures{};
		std::unordered_map<const char*, std::shared_ptr<System>> systems{};
	};
}

#endif