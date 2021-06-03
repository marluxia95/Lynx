#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdio.h>
#include <set>
#include "entity.h"

namespace Lynx::ECS {
	class System {
	public:
		std::set<Entity> entities;
	};
}

#endif 