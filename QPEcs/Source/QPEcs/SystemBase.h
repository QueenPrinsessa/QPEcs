#pragma once
#include "Entity.hpp"
#include "ComponentManager.hpp"
#include "Component.h"
#include <set>
namespace QPEcs
{
	class EntityComponentSystem;
	class SystemBase
	{
		friend class SystemManager;
		friend class EntityComponentSystem;
		public:
			virtual ~SystemBase() = default;

			template <class ... Components>
			void SetSignature();
		protected:
			std::set<Entity> myEntities {};
			EntityComponentSystem* myECS { nullptr };
		private:
			ComponentManager* myComponentManager;
			Signature mySignature;
	};

	template <class ... Components>
	void SystemBase::SetSignature()
	{
		assert(myComponentManager && "ComponentManager hasn't been set yet for system!");
		Signature signature;
		((signature.set(myComponentManager->GetComponentType<Components>())), ...);
		mySignature = signature;
	}
}
