#pragma once
#include "Entity.h"

namespace QPEcs
{
	class ComponentRegistryBase
	{
		public:
			virtual ~ComponentRegistryBase() = default;
			virtual void OnEntityDestroyed(Entity aEntity) = 0;
	};
}
