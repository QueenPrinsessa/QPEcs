#pragma once
#include "QPEcs/Entity.hpp"
#include <unordered_set>

namespace QPEcs
{
	class EntityComponentSystem;

	class GenericView
	{
		friend class ViewManager;
		public:
			virtual ~GenericView() = default;

			std::unordered_set<Entity>::iterator begin();
			std::unordered_set<Entity>::iterator end();
			std::unordered_set<Entity>::const_iterator begin() const;
			std::unordered_set<Entity>::const_iterator end() const;
		protected:
			std::unordered_set<Entity> myEntities {};
			EntityComponentSystem* myECS { nullptr };
	};

	inline std::unordered_set<Entity>::iterator GenericView::begin()
	{
		return myEntities.begin();
	}

	inline std::unordered_set<Entity>::iterator GenericView::end()
	{
		return myEntities.end();
	}

	inline std::unordered_set<Entity>::const_iterator GenericView::begin() const
	{
		return myEntities.begin();
	}

	inline std::unordered_set<Entity>::const_iterator GenericView::end() const
	{
		return myEntities.end();
	}
}
