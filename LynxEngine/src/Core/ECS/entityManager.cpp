#include <stdio.h>
#include <stdint.h>
#include "Core/assert.h"
#include "entityManager.h"

namespace Lynx::ECS {

Entity EntityManager::CreateEntity() {
	Entity newEnt = Entity();
	Signatures.push_back(Signature());
	newEnt = livingEntityCount++;
	return newEnt;
}

void EntityManager::DestroyEntity(Entity ent) {
	LYNX_ASSERT(ent < livingEntityCount, "Entity out of range.");
	livingEntityCount--;
	delete &ent;
}

void EntityManager::SetSignature(Entity ent, Signature signature) {
	LYNX_ASSERT(ent < livingEntityCount, "Entity out of range.");
	Signatures[ent] = signature;
}

Signature EntityManager::GetSignature(Entity ent) {
	LYNX_ASSERT(ent < livingEntityCount, "Entity out of range.");
	return Signatures[ent];
}

}