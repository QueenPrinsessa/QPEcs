#pragma once

#include "ComponentManager.hpp"
#include "SystemManager.hpp"
#include "EntityManager.hpp"

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
		inline bool HasComponent(Entity aEntity);

		template <class Component>
		inline void RegisterComponent();

		template <class Component>
		inline void AddComponent(Entity aEntity, Component aComponent);

		template <class Component>
		inline void AddAndRegisterComponent(Entity aEntity, Component aComponent);

		template <class Component>
		inline void RemoveComponent(Entity aEntity);

		template <class Component>
		inline Component& GetComponent(Entity aEntity);

		template <class Component>
		inline Component& GetOrAddComponent(Entity aEntity);

		template <class Component>
		inline ComponentType GetComponentType();

		template <class System>
		inline void RegisterSystem(bool aNotifyOfExistingEntities = true);

		template <class System>
		inline void SetSystemSignature(Signature aSignature);

		template <class System>
		inline std::shared_ptr<System> GetSystem();

		template <class System>
		inline std::shared_ptr<System> GetAndRegisterSystem();

	private:
		std::unique_ptr<EntityManager> myEntityManager;
		std::unique_ptr<ComponentManager> myComponentManager;
		std::unique_ptr<SystemManager> mySystemManager;
		void NotifySystemsOfAllEntities();
	};

	template <class Component>
	inline bool EntityComponentSystem::HasComponent(Entity aEntity)
	{
		Signature componentSignature;
		componentSignature.set(myComponentManager->GetComponentType<Component>());
		return (myEntityManager->GetSignature(aEntity) & componentSignature) == componentSignature;
	}

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
	inline void EntityComponentSystem::AddAndRegisterComponent(Entity aEntity, Component aComponent)
	{
		myComponentManager->AddAndRegisterComponent(aEntity, aComponent);

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
	inline Component& EntityComponentSystem::GetOrAddComponent(Entity aEntity)
	{
		auto signature = myEntityManager->GetSignature(aEntity);
		Component& component = myComponentManager->GetOrAddComponent<Component>();

		auto newSignature = signature;
		signature.set(myComponentManager->GetComponentType<Component>());

		if (signature != newSignature)
		{
			myEntityManager->SetSignature(aEntity, signature);
			mySystemManager->OnEntitySignatureChanged(aEntity, signature);
		}

		return component;
	}

	template <class Component>
	inline ComponentType EntityComponentSystem::GetComponentType()
	{
		return myComponentManager->GetComponentType<Component>();
	}

	template <class System>
	inline void EntityComponentSystem::RegisterSystem(bool aNotifyOfExistingEntities)
	{
		mySystemManager->RegisterSystem<System>(this);

		if (aNotifyOfExistingEntities)
		{
			NotifySystemsOfAllEntities();
		}
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

	template <class System>
	std::shared_ptr<System> EntityComponentSystem::GetAndRegisterSystem()
	{
		auto system = mySystemManager->GetAndRegisterSystem<System>();
		NotifySystemsOfAllEntities();
		return system;
	}

	inline EntityComponentSystem::EntityComponentSystem()
	{
		myComponentManager = std::make_unique<ComponentManager>();
		myEntityManager = std::make_unique<EntityManager>();
		mySystemManager = std::make_unique<SystemManager>();
	}

	inline Entity EntityComponentSystem::CreateEntity()
	{
		Entity entity = myEntityManager->CreateEntity();
		//entity.myECS = this;
		return entity;
	}

	inline void EntityComponentSystem::DestroyEntity(Entity aEntity)
	{
		myEntityManager->DestroyEntity(aEntity);
		myComponentManager->OnEntityDestroyed(aEntity);
		mySystemManager->OnEntityDestroyed(aEntity);
	}

	inline void EntityComponentSystem::NotifySystemsOfAllEntities()
	{
		for (EntityType entity = 0; entity < MaxEntities; entity++)
		{
			mySystemManager->OnEntitySignatureChanged(entity, myEntityManager->GetSignature(entity));
		}
	}
}
