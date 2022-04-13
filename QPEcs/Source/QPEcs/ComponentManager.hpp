#pragma once
#include "Entity.hpp"
#include "Types.h"
#include "ComponentRegistry.hpp"
#include <memory>
#include <unordered_map>

namespace QPEcs
{
	class ComponentManager
	{
		using TypeName = const char*;
		public:
			template <class Component>
			void RegisterComponent();

			template <class Component>
			ComponentType GetComponentType();

			template <class Component, class ... Args>
			void AddComponent(Entity aEntity, Args&&... aArgs);

			template <class Component>
			void RemoveComponent(Entity aEntity);

			template <class Component>
			Component& GetComponent(Entity aEntity);

			template <class Component>
			inline void CopyComponent(Entity aFrom, Entity aTo);

			template <class Component>
			bool IsRegistered();

			void OnEntityDestroyed(Entity aEntity);

		private:
			std::unordered_map<TypeName, ComponentType> myComponentTypes {};
			std::unordered_map<TypeName, std::shared_ptr<ComponentRegistryBase>> myComponentRegistries {};
			ComponentType myNextComponentType{};

			template <class Component>
			std::shared_ptr<ComponentRegistry<Component>> GetComponentRegistry();

			template <class Component>
			TypeName GetTypeName();
	};

	template <class Component>
	inline bool ComponentManager::IsRegistered()
	{
		return myComponentTypes.contains(GetTypeName<Component>());
	}

	inline void ComponentManager::OnEntityDestroyed(Entity aEntity)
	{
		for (auto const& [typeName, component] : myComponentRegistries)
		{
			component->OnEntityDestroyed(aEntity);
		}
	}

	template <class Component>
	void ComponentManager::RegisterComponent()
	{
		if (!IsRegistered<Component>())
		{
			assert(!myComponentTypes.contains(GetTypeName<Component>()) && "Component has already been registered!");

			myComponentTypes[GetTypeName<Component>()] = myNextComponentType++;
			myComponentRegistries[GetTypeName<Component>()] = std::make_shared<ComponentRegistry<Component>>();
		}
	}

	template <class Component>
	ComponentType ComponentManager::GetComponentType()
	{
		RegisterComponent<Component>();
		return myComponentTypes[GetTypeName<Component>()];
	}

	template <class Component, class ... Args>
	void ComponentManager::AddComponent(Entity aEntity, Args&&... aArgs)
	{
		GetComponentRegistry<Component>()->AddComponent(aEntity, std::forward<Args>(aArgs)...);
	}

	template <class Component>
	void ComponentManager::RemoveComponent(Entity aEntity)
	{
		GetComponentRegistry<Component>()->RemoveComponent(aEntity);
	}

	template <class Component>
	Component& ComponentManager::GetComponent(Entity aEntity)
	{
		return GetComponentRegistry<Component>()->GetComponent(aEntity);
	}


	template <class Component>
	void ComponentManager::CopyComponent(Entity aFrom, Entity aTo)
	{
		assert(myComponentTypes.contains(GetTypeName<Component>()) && "You need to register components before copying them!");

		GetComponentRegistry<Component>()->CopyComponent(aFrom, aTo);
	}

	template <class Component>
	std::shared_ptr<ComponentRegistry<Component>> ComponentManager::GetComponentRegistry()
	{
		assert(myComponentTypes.contains(GetTypeName<Component>()) && "You need to register components before using them!");

		return std::static_pointer_cast<ComponentRegistry<Component>>(myComponentRegistries[GetTypeName<Component>()]);
	}

	template <class Component>
	ComponentManager::TypeName ComponentManager::GetTypeName()
	{
		return typeid(Component).name();
	}
}
