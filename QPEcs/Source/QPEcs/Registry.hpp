#pragma once
#include "QPEcs/Types.h"
#include "QPEcs/Entity.hpp"

namespace QPEcs
{
	template <typename EntityType = EntityDefaultType>
	class Registry
	{
		public:

			Entity<EntityType> CreateEntity();

			void DestroyEntity(Entity<EntityType> aEntity);

			void RegisterComponent(Entity<EntityType> aEntity);

			template <class Component, typename... Args>
			void AddComponent(Entity<EntityType> aEntity, Args&& ... aArgs);

			template <class Component>
			void RemoveComponent(Entity<EntityType> aEntity);

			template <class Component>
			void GetComponent(Entity<EntityType> aEntity);

			template <class Component>
			void HasComponent(Entity<EntityType> aEntity);

		private:
			EntityType myNextId = 1;
	};

}
