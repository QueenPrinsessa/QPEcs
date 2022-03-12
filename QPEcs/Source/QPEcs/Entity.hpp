#pragma once
#include "Types.h"

namespace QPEcs
{
	constexpr EntityType MaxEntities = 4096;
	constexpr EntityType NullEntity = MaxEntities;

	using Entity = EntityType;
}
