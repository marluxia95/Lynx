#include "coordinator.h"

namespace Lynx {
	void Coordinator::Init() {
		componentManager = std::make_unique<ComponentManager>();
		entityManager = std::make_unique<EntityManager>();
		systemManager = std::make_unique<SystemManager>();
	}

	Entity Coordinator::CreateEntity() {
		return entityManager->CreateEntity();
	}

	void Coordinator::DestroyEntity(Entity entity) {
		entityManager->DestroyEntity(entity);
	}

	template<typename T>
	void Coordinator::RegisterComponent() {
		componentManager->RegisterComponent<T>();
	}

	template<typename T>
	void Coordinator::AddComponent(Entity entity, T component) {
		componentManager->AddComponent(entity, component);

		auto signature = entityManager->GetSignature(entity);
		signature.set(componentManager->GetComponentType<T>(), true);
		entityManager->SetSignature(entity, signature);

		systemManager->EntitySignatureChanged(entity, signature);
	}

	template<typename T>
	void Coordinator::RemoveComponent(Entity entity) {
		componentManager->RemoveComponent<T>(entity);

		auto signature = entityManager->GetSignature(entity);
		signature.set(componentManager->GetComponentType<T>(), false);
		entityManager->SetSignature(entity, signature);

		systemManager->EntitySignatureChanged(entity, signature);
	}

	template<typename T>
	T& Coordinator::GetComponent(Entity entity) {
		return componentManager->GetComponent<T>(entity);
	}

	template<typename T>
	ComponentType Coordinator::GetComponentType() {
		return componentManager->GetComponentType<T>();
	}

	template<typename T>
	std::shared_ptr<T> Coordinator::RegisterSystem() {
		return systemManager->RegisterSystem();
	}

	template<typename T>
	void Coordinator::SetSystemSignature(Signature signature) {
		systemManager->SetSignature<T>(signature);
	}
}