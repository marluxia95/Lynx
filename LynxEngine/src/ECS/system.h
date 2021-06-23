#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdio.h>
#include <set>
#include "entity.h"

namespace Lynx::ECS {
	class System {
		public:
			void Init() {};
			void Update() {};
			std::set<Entity> entities;
	};
}

#endif 