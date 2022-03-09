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

			Component& GetComponent(Entity aEntity);

			virtual void OnEntityDestroyed(Entity aEntity) override;

		private:
			std::array<Component*, MaxEntities> myComponents {};
			std::unordered_map<Entity, UInt64> myEntityToIndexMap {};
			std::unordered_map<UInt64, Entity> myIndexToEntityMap {};

			UInt64 mySize {};
	};

	template <typename Component>
	ComponentRegistry<Component>::ComponentRegistry()
	{
		for (auto component : myComponents)
		{
			component = nullptr;
		}
	}

	template <typename Component>
	ComponentRegistry<Component>::~ComponentRegistry()
	{
		for (auto component : myComponents)
		{
			delete component;
			component = nullptr;
		}
	}

	template <typename Component>
	template <typename ... Args>
	void ComponentRegistry<Component>::AddComponent(Entity aEntity, Args&&... aArgs)
	{
		assert(!myEntityToIndexMap.contains(aEntity) && "Entity already has component");

		UInt64 index = mySize++;
		myEntityToIndexMap[aEntity] = index;
		myIndexToEntityMap[index] = aEntity;
		myComponents[index] = new Component(std::forward<Args>(aArgs)...);
	}

	template <typename Component>
	void ComponentRegistry<Component>::RemoveComponent(Entity aEntity)
	{
		assert(myEntityToIndexMap.contains(aEntity) && "Removing component which doesn't exist!");

		UInt64 removedEntityIndex = myEntityToIndexMap[aEntity];
		UInt64 lastIndex = --mySize;

		delete myComponents[removedEntityIndex];
		myComponents[removedEntityIndex] = myComponents[lastIndex];
		myComponents[lastIndex] = nullptr;

		Entity lastEntity = myIndexToEntityMap[lastIndex];
		myEntityToIndexMap[lastEntity] = removedEntityIndex;
		myIndexToEntityMap[removedEntityIndex] = lastEntity;

		myEntityToIndexMap.erase(aEntity);
		myIndexToEntityMap.erase(lastIndex);
	}

	template <typename Component>
	Component& ComponentRegistry<Component>::GetComponent(Entity aEntity)
	{
		assert(myEntityToIndexMap.contains(aEntity) && "Entity does not have component");
		return *myComponents[myEntityToIndexMap[aEntity]];
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
