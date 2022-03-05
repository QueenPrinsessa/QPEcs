#pragma once
#include "Component.h"
#include "Entity.h"
#include "SystemBase.h"
#include <cassert>
#include <memory>
#include <unordered_map>

namespace QPEcs
{
	class SystemManager
	{
		using TypeName = const char*;
		public:
		template <class System>
		std::shared_ptr<System> RegisterSystem();

		template <class System>
		void SetSignature(Signature aSignature);

		void OnEntityDestroyed(Entity aEntity);

		void OnEntitySignatureChanged(Entity aEntity, Signature aEntitySignature);

		private:

			std::unordered_map<TypeName, Signature> mySignatures {};
			std::unordered_map<TypeName, std::shared_ptr<SystemBase>> mySystems {};

			template <class System>
			TypeName GetTypeName();
	};

	inline void SystemManager::OnEntityDestroyed(Entity aEntity)
	{
		for (auto const& [typeName, system] : mySystems)
		{
			system->myEntities.erase(aEntity);
		}
	}

	inline void SystemManager::OnEntitySignatureChanged(Entity aEntity, Signature aEntitySignature)
	{
		for (auto const& [typeName, system] : mySystems)
		{
			auto const& systemSignature = mySignatures[typeName];

			if((aEntitySignature & systemSignature) == systemSignature)
			{
				system->myEntities.insert(aEntity);
			}
			else
			{
				system->myEntities.erase(aEntity);
			}
		}
	}

	template <class System>
	std::shared_ptr<System> SystemManager::RegisterSystem()
	{
		static_assert(std::is_base_of_v<SystemBase, System>, "Systems need to inherit from QPEcs::SystemBase");
		assert(!mySystems.contains(GetTypeName<System>()) && "System has already been registered!");

		mySystems[GetTypeName<System>()] = std::make_shared<System>();
		return std::static_pointer_cast<System>(mySystems[GetTypeName<System>()]);
	}

	template <class System>
	void SystemManager::SetSignature(Signature aSignature)
	{
		static_assert(std::is_base_of_v<SystemBase, System>, "Systems need to inherit from QPEcs::SystemBase");
		assert(mySystems.contains(GetTypeName<System>()) && "System needs to be registered before use!");

		mySignatures[GetTypeName<System>()] = aSignature;
	}

	template <class System>
	SystemManager::TypeName SystemManager::GetTypeName()
	{
		static_assert(std::is_base_of_v<SystemBase, System>, "Systems need to inherit from QPEcs::SystemBase");
		return typeid(System).name();
	}
}
