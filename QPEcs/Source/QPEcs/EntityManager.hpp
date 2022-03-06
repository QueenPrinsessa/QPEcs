#pragma once
#include "Entity.h"
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

			Signature GetSignature(Entity aEntity);
			
		private:
			std::queue<Entity> myAvailableEntities {};
			std::array<Signature, MaxEntities> mySignatures {};
			UInt32 myEntitiesCount {};
			
	};


	inline EntityManager::EntityManager()
	{
		for (Entity entity = 0; entity < MaxEntities; entity++)
		{
			myAvailableEntities.push(entity);
		}
	}

	inline Entity EntityManager::CreateEntity()
	{
		assert(myEntitiesCount < MaxEntities && "Number of entities exceeding max entities!");

		Entity entity = myAvailableEntities.front();
		myAvailableEntities.pop();

		myEntitiesCount++;

		return entity;
	}

	inline void EntityManager::DestroyEntity(Entity aEntity)
	{
		assert(aEntity < MaxEntities && "Attempting to destroy entity out of range!");

		mySignatures[aEntity].reset();
		myAvailableEntities.push(aEntity);

		myEntitiesCount--;
	}

	inline void EntityManager::SetSignature(Entity aEntity, Signature aSignature)
	{
		assert(aEntity < MaxEntities && "Attempting to set signature for an entity out of range!");

		mySignatures[aEntity] = aSignature;
	}

	inline Signature EntityManager::GetSignature(Entity aEntity)
	{
		assert(aEntity < MaxEntities && "Attempting to get signature for an entity out of range!");

		return mySignatures[aEntity];
	}

}
