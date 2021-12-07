#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdio.h>
#include <set>
#include "Core/scene.h"
#include "common.h"

namespace Lynx::ECS {
	class System {
		public:
			System(Scene* scene) : scene(scene) {}

			void SetScene(Scene* scene) { scene = scene; }

			virtual void Init() {}
			virtual void Update() {}
			virtual void OnEntityAdded(EntityID ent) {}
			virtual void OnEntityRemoved(EntityID ent) {}
			
			std::set<EntityID> entities;
		protected:
			Scene* scene;

	};
}

#endif 