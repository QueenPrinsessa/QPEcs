#pragma once
#include "Entity.hpp"
#include <set>
namespace QPEcs
{
	class EntityComponentSystem;
	class SystemBase
	{
		friend class SystemManager;
		friend class EntityComponentSystem;
		protected:
			std::set<Entity> myEntities {};
			EntityComponentSystem* myECS { nullptr };
	};
}