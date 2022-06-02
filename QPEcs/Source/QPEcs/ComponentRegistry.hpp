#pragma once
#include "ComponentRegistryBase.h"
#include <array>
#include <cassert>
#include <unordered_map>
#include "Structures/SparseSet.hpp"
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
			std::array<Component, MaxEntities> myComponents {};
			SparseSet<int64_t> myEntityIndexSet { std::numeric_limits<EntityType>::max(), MaxEntities };
			/*std::unordered_map<Entity, uint32_t> myEntityToIndexMap {};
			std::unordered_map<uint32_t, Entity> myIndexToEntityMap {};

			uint32_t mySize {};*/
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
		assert(!myEntityIndexSet.Contains(aEntity) && "Entity already has component");

		myEntityIndexSet.Insert(aEntity);

		uint32_t index = myEntityIndexSet.Search(aEntity);
		myComponents[index] = Component(std::forward<Args>(aArgs)...);
	}

	template <typename Component>
	void ComponentRegistry<Component>::RemoveComponent(Entity aEntity)
	{
		assert(myEntityIndexSet.Contains(aEntity) && "Removing component which doesn't exist!");

		if(myEntityIndexSet.Contains(aEntity))
		{
			uint32_t removedEntityIndex = myEntityIndexSet.Search(aEntity);
			myEntityIndexSet.Erase(aEntity);
			myComponents[removedEntityIndex] = myComponents[myEntityIndexSet.Size()];
		}
	}
	
	template <typename Component>
	Component& ComponentRegistry<Component>::GetComponent(Entity aEntity)
	{
		assert(myEntityIndexSet.Contains(aEntity) && "Entity does not have component");
		return myComponents[myEntityIndexSet.Search(aEntity)];
	}

	template <typename Component>
	void ComponentRegistry<Component>::OnEntityDestroyed(Entity aEntity)
	{
		if (myEntityIndexSet.Contains(aEntity))
		{
			RemoveComponent(aEntity);
		}
	}
}
