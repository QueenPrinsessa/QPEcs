#pragma once
#include "Entity.h"
#include <set>
namespace QPEcs
{
	class SystemBase
	{
		friend class SystemManager;
		protected:
			std::set<Entity> myEntities;
	};
}