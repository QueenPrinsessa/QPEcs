#pragma once
#include <cstdint>

namespace QPEcs
{
	template <typename SizeType>
	class SparseSet
	{
		public:
			SparseSet(SizeType aMaxValue, SizeType aMaxCapacity);
			~SparseSet();

			SizeType Search(SizeType aValue);
			void Insert(SizeType aValue);
			void Delete(SizeType aValue);

			bool Contains(SizeType aValue) const;

			void Clear();

			SizeType Size() const;
			
		private:
			SizeType* mySparseArray { nullptr };
			SizeType* myDenseArray { nullptr };
			SizeType mySize { 0 };
			SizeType myDenseArrayCapacity { 0 };
			SizeType myMaxValue { 0 };
	};

	template <typename SizeType>
	SparseSet<SizeType>::SparseSet(SizeType aMaxValue, SizeType aMaxCapacity)
	{
		mySparseArray = new SizeType[aMaxValue + 1];
		myDenseArray = new SizeType[aMaxCapacity];
		mySize = 0;
		myDenseArrayCapacity = aMaxCapacity;
		myMaxValue = aMaxValue;
	}

	template <typename SizeType>
	SparseSet<SizeType>::~SparseSet()
	{
		delete[] mySparseArray;
		delete[] myDenseArray;
	}

	template <typename SizeType>
	SizeType SparseSet<SizeType>::Search(SizeType aValue)
	{
		if(aValue > myMaxValue)
		{
			return -1;
		}

		if(mySparseArray[aValue] < mySize && myDenseArray[mySparseArray[aValue]] == aValue)
		{
			return mySparseArray[aValue];
		}

		return -1;
	}

	template <typename SizeType>
	void SparseSet<SizeType>::Insert(SizeType aValue)
	{
		if(aValue > myMaxValue)
		{
			return;
		}

		if(mySize >= myDenseArrayCapacity)
		{
			return;
		}

		if(Search(aValue) != -1)
		{
			return;
		}

		myDenseArray[mySize] = aValue;
		mySparseArray[aValue] = mySize;

		mySize++;
	}

	template <typename SizeType>
	void SparseSet<SizeType>::Delete(SizeType aValue)
	{
		if(Search(aValue) == -1)
		{
			return;
		}

		SizeType temp = myDenseArray[mySize - 1];
		myDenseArray[mySparseArray[aValue]] = temp;
		mySparseArray[temp] = mySparseArray[aValue];

		mySize--;
	}

	template <typename SizeType>
	bool SparseSet<SizeType>::Contains(SizeType aValue) const
	{
		return Search(aValue) != -1;
	}

	template <typename SizeType>
	void SparseSet<SizeType>::Clear()
	{
		mySize = 0;
	}

	template <typename SizeType>
	SizeType SparseSet<SizeType>::Size() const
	{
		return mySize;
	}
}
