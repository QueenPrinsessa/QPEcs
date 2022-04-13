#pragma once
#include "ComponentRegistryBase.h"
#include <array>
#include <cassert>
#include <unordered_map>

namespace QPEcs
{
	template <typename Component>
	class ComponentRegistry : public ComponentRegistryBase
	{
		public:
			ComponentRegistry();
			virtual ~ComponentRegistry() override;

			template<typename ... Args>
			void AddComponent(Entity aEntity, Args&&... aArgs);

			void RemoveComponent(Entity aEntity);

			void CopyComponent(Entity aFrom, Entity aTo);

			Component& GetComponent(Entity aEntity);

			virtual void OnEntityDestroyed(Entity aEntity) override;

		private:
			std::array<Component, MaxEntities> myComponents {};
			std::unordered_map<Entity, UInt64> myEntityToIndexMap {};
			std::unordered_map<UInt64, Entity> myIndexToEntityMap {};

			UInt64 mySize {};
	};

	template <typename Component>
	ComponentRegistry<Component>::ComponentRegistry()
	{
	}

	template <typename Component>
	ComponentRegistry<Component>::~ComponentRegistry()
	{
	}

	template <typename Component>
	template <typename ... Args>
	void ComponentRegistry<Component>::AddComponent(Entity aEntity, Args&&... aArgs)
	{
		assert(!myEntityToIndexMap.contains(aEntity) && "Entity already has component");

		UInt64 index = mySize++;
		myEntityToIndexMap[aEntity] = index;
		myIndexToEntityMap[index] = aEntity;
		myComponents[index] = Component(std::forward<Args>(aArgs)...);
	}

	template <typename Component>
	void ComponentRegistry<Component>::RemoveComponent(Entity aEntity)
	{
		assert(myEntityToIndexMap.contains(aEntity) && "Removing component which doesn't exist!");

		UInt64 removedEntityIndex = myEntityToIndexMap[aEntity];
		UInt64 lastIndex = --mySize;

		myComponents[removedEntityIndex] = myComponents[lastIndex];

		Entity lastEntity = myIndexToEntityMap[lastIndex];
		myEntityToIndexMap[lastEntity] = removedEntityIndex;
		myIndexToEntityMap[removedEntityIndex] = lastEntity;

		myEntityToIndexMap.erase(aEntity);
		myIndexToEntityMap.erase(lastIndex);
	}

	template <typename Component>
	void ComponentRegistry<Component>::CopyComponent(Entity aFrom, Entity aTo)
	{
		assert(myEntityToIndexMap.contains(aFrom) && "The entity to copy from doesn't have component");
		assert(!myEntityToIndexMap.contains(aTo) && "The entity to copy to already has component");
		Component& componentToCopy = GetComponent(aFrom);

		UInt64 index = mySize++;
		myEntityToIndexMap[aTo] = index;
		myIndexToEntityMap[index] = aTo;
		myComponents[index] = Component(componentToCopy);
	}

	template <typename Component>
	Component& ComponentRegistry<Component>::GetComponent(Entity aEntity)
	{
		assert(myEntityToIndexMap.contains(aEntity) && "Entity does not have component");
		return myComponents[myEntityToIndexMap[aEntity]];
	}

	template <typename Component>
	void ComponentRegistry<Component>::OnEntityDestroyed(Entity aEntity)
	{
		if (myEntityToIndexMap.contains(aEntity))
		{
			RemoveComponent(aEntity);
		}
	}
}
