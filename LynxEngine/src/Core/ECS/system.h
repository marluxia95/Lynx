#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdio.h>
#include <set>
#include "Core/scene.h"
#include "entity.h"

namespace Lynx::ECS {
	class System {
		public:
			System(Scene* scene) : scene(scene) {}
			virtual void Init() {};
			virtual void Update() {};
			void SetScene(Scene* scene) { scene = scene; }
			std::set<Entity> entities;
		protected:
			Scene* scene;

	};
}

#endif 