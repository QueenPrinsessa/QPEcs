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
<<<<<<< HEAD
			std::unordered_map<Entity, uint32_t> myEntityToIndexMap {};
			std::unordered_map<uint32_t, Entity> myIndexToEntityMap {};

			uint32_t mySize {};
=======
			std::unordered_map<Entity, uint64_t> myEntityToIndexMap {};
			std::unordered_map<uint64_t, Entity> myIndexToEntityMap {};

			uint64_t mySize {};
>>>>>>> 77b038f958aeec28df2b9dbbbe2dd9d16990fa27
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

<<<<<<< HEAD
		uint32_t index = mySize++;
=======
		uint64_t index = mySize++;
>>>>>>> 77b038f958aeec28df2b9dbbbe2dd9d16990fa27
		myEntityToIndexMap[aEntity] = index;
		myIndexToEntityMap[index] = aEntity;
		myComponents[index] = Component(std::forward<Args>(aArgs)...);
	}

	template <typename Component>
	void ComponentRegistry<Component>::RemoveComponent(Entity aEntity)
	{
		assert(myEntityToIndexMap.contains(aEntity) && "Removing component which doesn't exist!");

<<<<<<< HEAD
		uint32_t removedEntityIndex = myEntityToIndexMap[aEntity];
		uint32_t lastIndex = --mySize;
=======
		uint64_t removedEntityIndex = myEntityToIndexMap[aEntity];
		uint64_t lastIndex = --mySize;
>>>>>>> 77b038f958aeec28df2b9dbbbe2dd9d16990fa27

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

<<<<<<< HEAD
		uint32_t index = mySize++;
=======
		uint64_t index = mySize++;
>>>>>>> 77b038f958aeec28df2b9dbbbe2dd9d16990fa27
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
