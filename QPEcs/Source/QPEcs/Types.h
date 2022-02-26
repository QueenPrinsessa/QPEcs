#pragma once
#include <unordered_map>

namespace QPEcs
{
	using Int8 = signed char;
	using Int16 = short;
	using Int32 = int;
	using Int64 = long long;

	using UInt8 = unsigned char;
	using UInt16 = unsigned short;
	using UInt32 = unsigned int;
	using UInt64 = unsigned long long;

	using EntityDefaultType = UInt64;
	using ComponentDefaultType = UInt64;

	template <typename Key, typename Value>
	using Dictionary = std::unordered_map<Key, Value>;

	constexpr UInt64 NullEntity = 0;
}