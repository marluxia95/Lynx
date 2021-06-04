#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdio.h>
#include <set>
#include "entity.h"

namespace Lynx::ECS {
	class System {
	public:
		typedef std::shared_ptr<System> ptr;
		std::set<Entity> entities;
	};
}

#endif 