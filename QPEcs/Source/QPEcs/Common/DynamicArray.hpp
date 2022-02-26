#pragma once
#include "QPEcs/Types.h"
#include <cassert>
#include <algorithm>

namespace QPEcs
{
	template <typename Type, typename SizeType = UInt64>
	class DynamicArray
	{
		public:
			DynamicArray() = default;
			~DynamicArray();
			DynamicArray(DynamicArray&& aOther) noexcept;
			DynamicArray(const DynamicArray& aOther);

			inline DynamicArray& operator=(const DynamicArray& aOther) noexcept;
			inline DynamicArray& operator=(DynamicArray&& aOther) noexcept;
			inline bool operator==(const DynamicArray& aOther) const noexcept;
			inline bool operator!=(const DynamicArray& aOther) const noexcept;

			inline Type& operator[](SizeType aIndex) noexcept;
			inline const Type& operator[](SizeType aIndex) const noexcept;

			inline void PushBack(const Type& aElement) noexcept;

			template<class ... Args>
			inline void EmplaceBack(Args... aArgs) noexcept;

			inline void RemoveAt(SizeType aIndex) noexcept;
			inline void Remove(SizeType aIndex) noexcept;
			inline void PopBack() noexcept;
			inline void Reserve(SizeType aNewCapacity) noexcept;

		private:
			Type* myElements { nullptr };
			SizeType mySize { 0 };
			SizeType myCapacity { 0 };
	};

	template <typename Type, typename SizeType>
	inline DynamicArray<Type, SizeType>::~DynamicArray()
	{
		delete[] myElements;
	}

	template <typename Type, typename SizeType>
	inline DynamicArray<Type, SizeType>::DynamicArray(DynamicArray&& aOther) noexcept
	{
		operator=(aOther);
	}

	template <typename Type, typename SizeType>
	DynamicArray<Type, SizeType>::DynamicArray(const DynamicArray& aOther)
	{
		operator=(aOther);
	}

	template <typename Type, typename SizeType>
	inline DynamicArray<Type, SizeType>& DynamicArray<Type, SizeType>::operator=(const DynamicArray& aOther) noexcept
	{
		Type* dummyElements = new Type[aOther.myCapacity];
		for (SizeType i = 0; i < aOther.myCapacity; ++i)
		{
			dummyElements[i] = aOther[i];
		}
		delete[] myElements;

		myElements = dummyElements;
		mySize = aOther.mySize;
		myCapacity = aOther.myCapacity;

		return *this;
	}

	template <typename Type, typename SizeType>
	inline DynamicArray<Type, SizeType>& DynamicArray<Type, SizeType>::operator=(DynamicArray&& aOther) noexcept
	{
		myElements = std::move(aOther.myElements);
		mySize = std::move(aOther.mySize);
		myCapacity = std::move(aOther.myCapacity);

		return *this;
	}

	template <typename Type, typename SizeType>
	bool DynamicArray<Type, SizeType>::operator==(const DynamicArray& aOther) const noexcept
	{
		if(mySize != aOther.mySize)
		{
			return false;
		}

		for (SizeType i = 0; i < mySize; ++i)
		{
			if(aOther.myElements[i] != myElements[i])
			{
				return false;
			}
		}

		return true;
	}

	template <typename Type, typename SizeType>
	bool DynamicArray<Type, SizeType>::operator!=(const DynamicArray& aOther) const noexcept
	{
		return !operator==(aOther);
	}

	template <typename Type, typename SizeType>
	inline Type& DynamicArray<Type, SizeType>::operator[](SizeType aIndex) noexcept
	{
		assert(aIndex < mySize && "Index out of bounds!");
		return myElements[aIndex];
	}

	template <typename Type, typename SizeType>
	const Type& DynamicArray<Type, SizeType>::operator[](SizeType aIndex) const noexcept
	{
		return operator[](aIndex);
	}

	template <typename Type, typename SizeType>
	inline void DynamicArray<Type, SizeType>::PushBack(const Type& aElement) noexcept
	{

		if(++mySize == myCapacity)
		{
			Reserve(myCapacity * 2);
		}

		myElements[mySize] = aElement;
	}

	template <typename Type, typename SizeType>
	template <class ... Args>
	inline void DynamicArray<Type, SizeType>::EmplaceBack(Args... aArgs) noexcept
	{
		if (++mySize == myCapacity)
		{
			Reserve(myCapacity * 2);
		}

		myElements[mySize] = Type(aArgs);
	}

	template <typename Type, typename SizeType>
	void DynamicArray<Type, SizeType>::RemoveAt(SizeType aIndex) noexcept
	{
		assert(aIndex < mySize && "Index out of bounds!");

		memcpy(myElements + aIndex, myElements + aIndex + 1, (mySize - (aIndex + 1)) * sizeof(Type));
	}

	template <typename Type, typename SizeType>
	void DynamicArray<Type, SizeType>::Remove(SizeType aIndex) noexcept
	{
		for (SizeType i = 0; i < mySize; ++i)
		{
			if(aIndex == myElements[i])
			{
				RemoveAt(i);
				return;
			}
		}
	}

	template <typename Type, typename SizeType>
	void DynamicArray<Type, SizeType>::PopBack() noexcept
	{
		--mySize;
	}

	template <typename Type, typename SizeType>
	void DynamicArray<Type, SizeType>::Reserve(SizeType aNewCapacity) noexcept
	{
		assert(aNewCapacity >= myCapacity);

		Type* dummyElements = new Type[aNewCapacity];
		memcpy(dummyElements, myElements, mySize * sizeof(Type));
		delete[] myElements;
		myElements = dummyElements;
	}

}
