#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdio.h>
#include <memory>
#include <set>
#include "Core/scene.h"
#include "common.h"

namespace Lynx::ECS {
	class System {
		public:
			System(std::shared_ptr<Lynx::Scene> scene) : scene(scene) {}

			void SetScene(std::shared_ptr<Lynx::Scene> n_scene) { scene = n_scene; }

			virtual void Init() {}
			virtual void Update() {}
			virtual void OnEntityAdded(EntityID ent) {}
			virtual void OnEntityRemoved(EntityID ent) {}
			
			std::set<EntityID> entities;
		protected:
			std::shared_ptr<Lynx::Scene> scene;

	};
}

#endif 