#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <stdio.h>
#include <stdint.h>
#include <vector>
#include <queue>
#include "entity.h"

namespace Lynx {
	class EntityManager {
		public:
			Entity CreateEntity();
			void DestroyEntity(Entity ent);
			void SetSignature(Entity ent, Signature signature);
			Signature GetSignature(Entity ent);
		private:

			std::vector<Signature> Signatures;
			uint64_t livingEntityCount = 0;
	};
}

#endif