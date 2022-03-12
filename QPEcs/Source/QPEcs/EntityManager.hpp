#pragma once
#include "Entity.hpp"
#include "Component.h"
#include <queue>
#include <array>
#include <cassert>

namespace QPEcs
{
	class EntityManager
	{
		friend class EntityComponentSystem;
		public:
			EntityManager();
			~EntityManager() = default;

			Entity CreateEntity();

			void DestroyEntity(Entity aEntity);

			void SetSignature(Entity aEntity, Signature aSignature);

			Signature GetSignature(Entity aEntity) const;

			bool IsValid(Entity aEntity) const;
			
		private:
			std::bitset<MaxEntities> myEntities {};
			std::array<Signature, MaxEntities> mySignatures {};
			UInt32 myEntitiesCount {};
			
	};

	inline bool EntityManager::IsValid(Entity aEntity) const
	{
		return myEntities[aEntity];
	}


	inline EntityManager::EntityManager()
	{
	}

	inline Entity EntityManager::CreateEntity()
	{
		assert(myEntitiesCount < MaxEntities && "Number of entities exceeding max entities!");

		Entity entity = 0;
		for (EntityType nextEntity = 0; nextEntity < MaxEntities; nextEntity++)
		{
			if(!myEntities[nextEntity])
			{
				entity = nextEntity;
				break;
			}
		}

		myEntities.set(entity);
		myEntitiesCount++;

		return entity;
	}

	inline void EntityManager::DestroyEntity(Entity aEntity)
	{
		assert(aEntity < MaxEntities && "Attempting to destroy entity out of range!");

		myEntities.reset(aEntity);
		mySignatures[aEntity].reset();

		myEntitiesCount--;
	}

	inline void EntityManager::SetSignature(Entity aEntity, Signature aSignature)
	{
		assert(aEntity < MaxEntities && "Attempting to set signature for an entity out of range!");

		mySignatures[aEntity] = aSignature;
	}

	inline Signature EntityManager::GetSignature(Entity aEntity) const
	{
		assert(aEntity < MaxEntities && "Attempting to get signature for an entity out of range!");

		return mySignatures[aEntity];
	}

}
