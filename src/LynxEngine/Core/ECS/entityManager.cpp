#include <stdio.h>
#include <stdint.h>
#include "Core/assert.h"
#include "entityManager.h"

namespace Lynx::ECS {

EntityID EntityManager::CreateEntity() {
	EntityID newEnt = EntityID();
	Signatures.push_back(Signature());
	newEnt = livingEntityCount++;
	return newEnt;
}

void EntityManager::DestroyEntity(EntityID ent) {
	LYNX_ASSERT(ent < livingEntityCount, "Entity out of range.");
	livingEntityCount--;
	delete &ent;
}

void EntityManager::SetSignature(EntityID ent, const Signature signature) {
	LYNX_ASSERT(ent < livingEntityCount, "Entity out of range.");
	Signatures[ent] = signature;
}

Signature EntityManager::GetSignature(EntityID ent) {
	LYNX_ASSERT(ent < livingEntityCount, "Entity out of range.");
	return Signatures[ent];
}

}