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

			inline Entity CreateEntity();

			inline void DestroyEntity(Entity aEntity);

			template <class Component>
			inline void RegisterComponent();

			template <class Component>
			inline void AddComponent(Entity aEntity, Component aComponent);

			template <class Component>
			inline void RemoveComponent(Entity aEntity);

			template <class Component>
			inline Component& GetComponent(Entity aEntity);

			template <class Component>
			inline ComponentType GetComponentType();

			template <class System>
			inline void RegisterSystem();

			template <class System>
			inline void SetSystemSignature(Signature aSignature);

			template <class System>
			inline std::shared_ptr<System> GetSystem();

		private:
			std::unique_ptr<EntityManager> myEntityManager;
			std::unique_ptr<ComponentManager> myComponentManager;
			std::unique_ptr<SystemManager> mySystemManager;
	};

	template <class Component>
	inline void EntityComponentSystem::RegisterComponent()
	{
		myComponentManager->RegisterComponent<Component>();
	}

	template <class Component>
	inline void EntityComponentSystem::AddComponent(Entity aEntity, Component aComponent)
	{
		myComponentManager->AddComponent(aEntity, aComponent);

		auto signature = myEntityManager->GetSignature(aEntity);
		signature.set(myComponentManager->GetComponentType<Component>());
		myEntityManager->SetSignature(aEntity, signature);

		mySystemManager->OnEntitySignatureChanged(aEntity, signature);
	}

	template <class Component>
	inline void EntityComponentSystem::RemoveComponent(Entity aEntity)
	{
		myComponentManager->RemoveComponent<Component>(aEntity);

		auto signature = myEntityManager->GetSignature(aEntity);
		signature.reset(myComponentManager->GetComponentType<Component>());
		myEntityManager->SetSignature(aEntity, signature);

		mySystemManager->OnEntitySignatureChanged(aEntity, signature);
	}

	template <class Component>
	inline Component& EntityComponentSystem::GetComponent(Entity aEntity)
	{
		return myComponentManager->GetComponent<Component>(aEntity);
	}

	template <class Component>
	inline ComponentType EntityComponentSystem::GetComponentType()
	{
		return myComponentManager->GetComponentType<Component>();
	}

	template <class System>
	inline void EntityComponentSystem::RegisterSystem()
	{
		mySystemManager->RegisterSystem<System>(this);
	}

	template <class System>
	inline void EntityComponentSystem::SetSystemSignature(Signature aSignature)
	{
		mySystemManager->SetSignature<System>(aSignature);
	}

	template <class System>
	inline std::shared_ptr<System> EntityComponentSystem::GetSystem()
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
