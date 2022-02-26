#pragma once
#include "QPEcs/Types.h"

namespace QPEcs
{
	template <typename UnderlyingType = EntityDefaultType>
	class Entity
	{
		friend class Registry;
	public:
		Entity() = default;
		~Entity() = default;
		Entity(const Entity&) = default;
		Entity(Entity&&) = default;

		Entity& operator=(Entity&&) noexcept = default;
		Entity& operator=(const Entity& aOther) noexcept = default;

		inline explicit operator UnderlyingType() const noexcept;

		inline bool operator==(const Entity& aOther) const noexcept;
		inline bool operator!=(const Entity& aOther) const noexcept;

		inline UnderlyingType GetId() const noexcept;

	private:
		explicit Entity(UnderlyingType aId);
		UnderlyingType myId { NullEntity };
	};

	template <typename UnderlyingType>
	Entity<UnderlyingType>::operator UnderlyingType() const noexcept
	{
		return myId;
	}

	template <typename UnderlyingType>
	bool Entity<UnderlyingType>::operator==(const Entity& aOther) const noexcept
	{
		return myId == aOther.myId;
	}

	template <typename UnderlyingType>
	bool Entity<UnderlyingType>::operator!=(const Entity& aOther) const noexcept
	{
		return !operator==(aOther);
	}

	template <typename UnderlyingType>
	UnderlyingType Entity<UnderlyingType>::GetId() const noexcept
	{
		return myId;
	}

	template <typename UnderlyingType>
	Entity<UnderlyingType>::Entity(UnderlyingType aId)
		: myId(aId)
	{
	}
}
