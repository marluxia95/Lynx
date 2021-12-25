#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <stdio.h>
#include <stdint.h>
#include <vector>
#include <queue>
#include "common.h"

namespace Lynx::ECS {

	class LYNXENGINE_API EntityManager {
		public:
			~EntityManager()
			{
				Signatures.clear();
			}

			EntityID CreateEntity();
			
			void DestroyEntity(EntityID ent);
			
			void SetSignature(EntityID ent, Signature signature);
			
			Signature GetSignature(EntityID ent);
			
			uint64_t livingEntityCount = 0;
		
		private:
			std::vector<Signature> Signatures;

	};
}

#endif