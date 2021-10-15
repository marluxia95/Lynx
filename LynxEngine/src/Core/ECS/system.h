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

			void SetScene(Scene* scene) { scene = scene; }

			virtual void Init() {}
			virtual void Update() {}
			virtual void OnEntityAdded(Entity ent) {}
			virtual void OnEntityRemoved(Entity ent) {}
			
			std::set<Entity> entities;
		protected:
			Scene* scene;

	};
}

#endif 