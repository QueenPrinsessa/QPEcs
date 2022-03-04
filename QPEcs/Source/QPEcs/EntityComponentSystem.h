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

			template <typename Component>
			void RegisterComponent();

			template <typename Component>
			void AddComponent(Entity aEntity, Component aComponent);

			template <typename Component>
			void RemoveComponent(Entity aEntity);

			template <typename Component>
			Component& GetComponent(Entity aEntity);

			template <typename Component>
			ComponentType GetComponentType();

			template <typename System>
			std::shared_ptr<System> RegisterSystem();

			template <typename System>
			void SetSystemSignature(Signature aSignature);

		private:
			std::unique_ptr<EntityManager> myEntityManager;
			std::unique_ptr<ComponentManager> myComponentManager;
			std::unique_ptr<SystemManager> mySystemManager;
	};

	template <typename Component>
	void EntityComponentSystem::RegisterComponent()
	{
		myComponentManager->RegisterComponent<Component>();
	}

	template <typename Component>
	void EntityComponentSystem::AddComponent(Entity aEntity, Component aComponent)
	{
		myComponentManager->AddComponent(aEntity, aComponent);
	}

	template <typename Component>
	void EntityComponentSystem::RemoveComponent(Entity aEntity)
	{
		myComponentManager->RemoveComponent<Component>(aEntity);
	}

	template <typename Component>
	Component& EntityComponentSystem::GetComponent(Entity aEntity)
	{
		return myComponentManager->GetComponent<Component>(aEntity);
	}

	template <typename Component>
	ComponentType EntityComponentSystem::GetComponentType()
	{
		return myComponentManager->GetComponentType<Component>();
	}

	template <typename System>
	std::shared_ptr<System> EntityComponentSystem::RegisterSystem()
	{
		return mySystemManager->RegisterSystem<System>();
	}

	template <typename System>
	void EntityComponentSystem::SetSystemSignature(Signature aSignature)
	{
		mySystemManager->SetSignature<System>(aSignature);
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
