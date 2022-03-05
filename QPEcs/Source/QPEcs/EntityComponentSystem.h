#pragma once
#include "EntityManager.hpp"
#include "ComponentManager.hpp"
#include "SystemManager.hpp"

namespace QPEcs
{
	class EntityComponentSystem
	{
		public:
			EntityComponentSystem();
			~EntityComponentSystem() = default;

			Entity CreateEntity();

			void DestroyEntity(Entity aEntity);

			template <class Component>
			void RegisterComponent();

			template <class Component>
			void AddComponent(Entity aEntity, Component aComponent);

			template <class Component>
			void RemoveComponent(Entity aEntity);

			template <class Component>
			Component& GetComponent(Entity aEntity);

			template <class Component>
			ComponentType GetComponentType();

			template <class System>
			void RegisterSystem();

			template <class System>
			void SetSystemSignature(Signature aSignature);

			template <class System>
			std::shared_ptr<System> GetSystem();

		private:
			std::unique_ptr<EntityManager> myEntityManager;
			std::unique_ptr<ComponentManager> myComponentManager;
			std::unique_ptr<SystemManager> mySystemManager;
	};

	template <class Component>
	void EntityComponentSystem::RegisterComponent()
	{
		myComponentManager->RegisterComponent<Component>();
	}

	template <class Component>
	void EntityComponentSystem::AddComponent(Entity aEntity, Component aComponent)
	{
		myComponentManager->AddComponent(aEntity, aComponent);

		auto signature = myEntityManager->GetSignature(aEntity);
		signature.set(myComponentManager->GetComponentType<Component>());
		myEntityManager->SetSignature(aEntity, signature);

		mySystemManager->OnEntitySignatureChanged(aEntity, signature);
	}

	template <class Component>
	void EntityComponentSystem::RemoveComponent(Entity aEntity)
	{
		myComponentManager->RemoveComponent<Component>(aEntity);

		auto signature = myEntityManager->GetSignature(aEntity);
		signature.reset(myComponentManager->GetComponentType<Component>());
		myEntityManager->SetSignature(aEntity, signature);

		mySystemManager->OnEntitySignatureChanged(aEntity, signature);
	}

	template <class Component>
	Component& EntityComponentSystem::GetComponent(Entity aEntity)
	{
		return myComponentManager->GetComponent<Component>(aEntity);
	}

	template <class Component>
	ComponentType EntityComponentSystem::GetComponentType()
	{
		return myComponentManager->GetComponentType<Component>();
	}

	template <class System>
	void EntityComponentSystem::RegisterSystem()
	{
		mySystemManager->RegisterSystem<System>();
	}

	template <class System>
	void EntityComponentSystem::SetSystemSignature(Signature aSignature)
	{
		mySystemManager->SetSignature<System>(aSignature);
	}

	template <class System>
	std::shared_ptr<System> EntityComponentSystem::GetSystem()
	{
		return mySystemManager->GetSystem<System>();
	}

	inline EntityComponentSystem::EntityComponentSystem()
	{
		myComponentManager = std::make_unique<ComponentManager>();
		myEntityManager = std::make_unique<EntityManager>();
		mySystemManager = std::make_unique<SystemManager>();
	}

	inline Entity EntityComponentSystem::CreateEntity()
	{
		return myEntityManager->CreateEntity();
	}

	inline void EntityComponentSystem::DestroyEntity(Entity aEntity)
	{
		myEntityManager->DestroyEntity(aEntity);
		myComponentManager->OnEntityDestroyed(aEntity);
		mySystemManager->OnEntityDestroyed(aEntity);
	}
}
