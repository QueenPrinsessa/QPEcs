#pragma once
#include "GenericView.hpp"
#include "QPEcs/EntityComponentSystem.hpp"
#include <tuple>

namespace QPEcs
{
	template <class ... Components>
	class View : public GenericView
	{
		public:
			virtual ~View() override = default;

			std::tuple<Components&...> Get(Entity aEntity) const;
	};

	template <class ... Components>
	std::tuple<Components&...> View<Components...>::Get(Entity aEntity) const
	{
		return std::tie(myECS->GetComponent<Components>(aEntity)...);
	}

}
