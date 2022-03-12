#pragma once
#include "GenericView.hpp"
#include "QPEcs/EntityComponentSystem.hpp"
#include <tuple>

namespace QPEcs
{
	template <class ... Components>
	class View : public GenericView
	{
		static_assert(sizeof...(Components) > 0, "A view can't consist of 0 components!");
		public:
			virtual ~View() override = default;

			auto Get(Entity aEntity) const;
	};

	template <class ... Components>
	auto View<Components...>::Get(Entity aEntity) const
	{
		if constexpr(sizeof...(Components) == 1)
		{
			return myECS->GetComponent<Components...>(aEntity);
		}
		else
		{
			return std::tie(myECS->GetComponent<Components>(aEntity)...);
		}
	}

}
