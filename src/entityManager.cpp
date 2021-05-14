#include <stdio.h>
#include <stdint.h>
#include <cassert>
#include "entityManager.h"

namespace Lynx {

Entity EntityManager::CreateEntity() {
	Entity newEnt = Entity();
	Signatures.push_back(Signature());
	livingEntityCount++;
	newEnt.id = livingEntityCount;
	return newEnt;
}

void EntityManager::DestroyEntity(Entity ent) {
	assert(ent.id > livingEntityCount && "Entity out of range.");
	livingEntityCount--;
	delete &ent;
}

void EntityManager::SetSignature(Entity ent, Signature signature) {
	assert(ent.id > livingEntityCount && "Entity out of range.");
	Signatures[ent.id] = signature;
}

Signature EntityManager::GetSignature(Entity ent) {
	assert(ent.id > livingEntityCount && "Entity out of range.");
	return Signatures[ent.id];
}

}