#pragma once

#include "EntityManager.hpp"
#include "ComponentManager.hpp"
#include "SystemManager.hpp"
#include "Views/ViewManager.hpp"

namespace QPEcs
{
	template <class ... Components>
	class View;

	class EntityComponentSystem
	{
	public:
		EntityComponentSystem();
		~EntityComponentSystem() = default;

		inline Entity CreateEntity();

		inline void DestroyEntity(Entity aEntity);

		bool IsValidEntity(Entity aEntity);

		template <class Component>
		inline bool HasComponent(Entity aEntity);

		template <class Component>
		inline bool IsComponentRegistered();

		template <class System>
		inline bool IsSystemRegistered();

		template <class Component>
		inline void RegisterComponent();

		template <class Component, typename ... Args>
		inline void AddComponent(Entity aEntity, Args&&... aArgs);

		template <class Component>
		inline void RemoveComponent(Entity aEntity);

		template <class Component>
		inline void CopyComponent(Entity aFrom, Entity aTo);

		template <class ... Components>
		inline void CopyComponents(Entity aFrom, Entity aTo);

		template <class Component>
		inline void TryCopyComponent(Entity aFrom, Entity aTo);

		template <class ... Components>
		inline void TryCopyComponents(Entity aFrom, Entity aTo);

		template <class Component>
		inline Component& GetComponent(Entity aEntity);

		template <class Component, typename ... Args>
		inline Component& GetOrAddComponent(Entity aEntity, Args&&... aArgs);

		template <class Component>
		inline ComponentType GetComponentType();

		template <class System>
		inline void RegisterSystem(bool aNotifyOfExistingEntities = true);

		template <class System>
		inline void SetSystemSignature(Signature aSignature);

		template <class System, class... Components>
		inline void SetSystemSignature();

		template <class System>
		inline std::shared_ptr<System> GetSystem();

		template <class System>
		inline std::shared_ptr<System> GetAndRegisterSystem(bool aNotifyOfExistingEntities = true);

		template <class ... Components>
		inline View<Components...>& GetView();

	private:
		std::unique_ptr<EntityManager> myEntityManager;
		std::unique_ptr<ComponentManager> myComponentManager;
		std::unique_ptr<SystemManager> mySystemManager;
		std::unique_ptr<ViewManager> myViewManager;
		void NotifySystemsOfAllEntities();
		void NotifyViewsOfAllEntities();
	};

	inline bool EntityComponentSystem::IsValidEntity(Entity aEntity)
	{
		return myEntityManager->IsValid(aEntity);
	}

	template <class Component>
	inline bool EntityComponentSystem::IsComponentRegistered()
	{
		return myComponentManager->IsRegistered<Component>();
	}

	template <class System>
	inline bool EntityComponentSystem::IsSystemRegistered()
	{
		return mySystemManager->IsRegistered<System>();
	}

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

	template <class Component, typename ... Args>
	inline void EntityComponentSystem::AddComponent(Entity aEntity, Args&&... aArgs)
	{
		myComponentManager->AddComponent<Component>(aEntity, std::forward<Args>(aArgs)...);

		auto signature = myEntityManager->GetSignature(aEntity);
		signature.set(myComponentManager->GetComponentType<Component>());
		myEntityManager->SetSignature(aEntity, signature);

		mySystemManager->OnEntitySignatureChanged(aEntity, signature);
		myViewManager->OnEntitySignatureChanged(aEntity, signature);
	}

	template <class Component>
	inline void EntityComponentSystem::RemoveComponent(Entity aEntity)
	{
		myComponentManager->RemoveComponent<Component>(aEntity);

		auto signature = myEntityManager->GetSignature(aEntity);
		signature.reset(myComponentManager->GetComponentType<Component>());
		myEntityManager->SetSignature(aEntity, signature);

		mySystemManager->OnEntitySignatureChanged(aEntity, signature);
		myViewManager->OnEntitySignatureChanged(aEntity, signature);
	}

	template <class Component>
	void EntityComponentSystem::CopyComponent(Entity aFrom, Entity aTo)
	{
		myComponentManager->CopyComponent<Component>(aFrom, aTo);

		auto signature = myEntityManager->GetSignature(aTo);
		signature.set(myComponentManager->GetComponentType<Component>());
		myEntityManager->SetSignature(aTo, signature);

		mySystemManager->OnEntitySignatureChanged(aTo, signature);
		myViewManager->OnEntitySignatureChanged(aTo, signature);
	}

	template <class ... Components>
	void EntityComponentSystem::CopyComponents(Entity aFrom, Entity aTo)
	{
		((CopyComponent<Components>(aFrom, aTo)), ...);
	}

	template <class Component>
	void EntityComponentSystem::TryCopyComponent(Entity aFrom, Entity aTo)
	{
		if(!HasComponent<Component>(aFrom))
		{
			return;
		}

		CopyComponent<Component>(aFrom, aTo);
	}

	template <class ... Components>
	void EntityComponentSystem::TryCopyComponents(Entity aFrom, Entity aTo)
	{
		((TryCopyComponent<Components>(aFrom, aTo)), ...);
	}

	template <class Component>
	inline Component& EntityComponentSystem::GetComponent(Entity aEntity)
	{
		return myComponentManager->GetComponent<Component>(aEntity);
	}

	template <class Component, typename ... Args>
	inline Component& EntityComponentSystem::GetOrAddComponent(Entity aEntity, Args&&... aArgs)
	{
		auto signature = myEntityManager->GetSignature(aEntity);
		Component& component = myComponentManager->GetOrAddComponent<Component>(std::forward<Args>(aArgs)...);

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

	template <class System, class ... Components>
	void EntityComponentSystem::SetSystemSignature()
	{
		Signature signature;
		((signature.set(GetComponentType<Components>())),...);
		mySystemManager->SetSignature<System>(signature);
	}

	template <class System>
	inline std::shared_ptr<System> EntityComponentSystem::GetSystem()
	{
		return mySystemManager->GetSystem<System>();
	}

	template <class System>
	std::shared_ptr<System> EntityComponentSystem::GetAndRegisterSystem(bool aNotifyOfExistingEntities)
	{
		auto system = mySystemManager->GetAndRegisterSystem<System>(this);

		if(aNotifyOfExistingEntities)
		{
			NotifySystemsOfAllEntities();
		}

		return system;
	}

	template <class ... Components>
	View<Components...>& EntityComponentSystem::GetView()
	{
		if(!myViewManager->IsRegistered<Components...>())
		{
			myViewManager->RegisterView<Components...>(this);
			NotifyViewsOfAllEntities();
		}
		return *myViewManager->GetView<Components...>(this);
	}

	inline EntityComponentSystem::EntityComponentSystem()
	{
		myComponentManager = std::make_unique<ComponentManager>();
		myEntityManager = std::make_unique<EntityManager>();
		mySystemManager = std::make_unique<SystemManager>(myComponentManager.get());
		myViewManager = std::make_unique<ViewManager>(myComponentManager.get());
	}

	inline Entity EntityComponentSystem::CreateEntity()
	{
		return  myEntityManager->CreateEntity();
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
			if(myEntityManager->myEntities[entity])
			{
				mySystemManager->OnEntitySignatureChanged(entity, myEntityManager->GetSignature(entity));
			}
		}
	}

	inline void EntityComponentSystem::NotifyViewsOfAllEntities()
	{
		for (EntityType entity = 0; entity < MaxEntities; entity++)
		{
			if (myEntityManager->myEntities[entity])
			{
				myViewManager->OnEntitySignatureChanged(entity, myEntityManager->GetSignature(entity));
			}
		}
	}
}
