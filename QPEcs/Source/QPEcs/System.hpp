#pragma once
#include "Entity.hpp"
#include "EntityComponentSystem.hpp"
#include <set>

namespace QPEcs
{
	class EntityComponentSystem;

	class SystemBase
	{
		friend class SystemManager;
		friend class EntityComponentSystem;
		protected:
			std::set<Entity> myEntities {};
			EntityComponentSystem* myECS { nullptr };
	};

	template <class Derived>
	class System : public SystemBase
	{
		public:
			template <class ... Components>
			void SetSignature();
	};

	template <class Derived>
	template <class ... Components>
	void System<Derived>::SetSignature()
	{
		myECS->SetSystemSignature<Derived, Components>();
	}
}
