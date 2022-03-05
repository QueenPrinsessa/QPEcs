#pragma once
#include "Entity.h"
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