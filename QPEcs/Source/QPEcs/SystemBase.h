#pragma once
#include "Entity.h"
#include <set>
namespace QPEcs
{
	class SystemBase
	{
		friend class SystemManager;
		std::set<Entity> myEntities;
	};
}