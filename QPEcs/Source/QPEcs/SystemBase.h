#pragma once
#include "Entity.h"
#include <set>
namespace QPEcs
{
	class SystemBase
	{
		friend class SystemManager;
		private:
			std::set<Entity> myEntities;
	};
}