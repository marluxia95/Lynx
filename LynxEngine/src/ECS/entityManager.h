#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <stdio.h>
#include <stdint.h>
#include <vector>
#include <queue>
#include "entity.h"

namespace Lynx::ECS {
	class EntityManager {
		public:
			~EntityManager()
			{
				Signatures.clear();
			}
			Entity CreateEntity();
			void DestroyEntity(Entity ent);
			void SetSignature(Entity ent, Signature signature);
			Signature GetSignature(Entity ent);
			uint64_t livingEntityCount = 0;
		private:

			std::vector<Signature> Signatures;

	};
}

#endif