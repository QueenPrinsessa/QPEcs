#pragma once
#include "Component.h"
#include "Entity.hpp"
#include "System.hpp"
#include <cassert>
#include <memory>
#include <unordered_map>

class EntityComponentSystem;
namespace QPEcs
{
	class SystemManager
	{
		using TypeName = const char*;
		public:
			template <class DerivedSystem>
			inline void RegisterSystem(EntityComponentSystem* aECS);

			template <class DerivedSystem>
			inline void SetSignature(Signature aSignature);

			inline void OnEntityDestroyed(Entity aEntity);

			inline void OnEntitySignatureChanged(Entity aEntity, Signature aEntitySignature);

			template <class DerivedSystem>
			inline std::shared_ptr<DerivedSystem> GetSystem();

			template <class DerivedSystem>
			inline std::shared_ptr<DerivedSystem> GetAndRegisterSystem(EntityComponentSystem* aECS);
		private:
			std::unordered_map<TypeName, Signature> mySignatures {};
			std::unordered_map<TypeName, std::shared_ptr<SystemBase>> mySystems {};

			template <class DerivedSystem>
			inline TypeName GetTypeName();
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

			if ((aEntitySignature & systemSignature) == systemSignature)
			{
				system->myEntities.insert(aEntity);
			}
			else
			{
				system->myEntities.erase(aEntity);
			}
		}
	}

	template <class DerivedSystem>
	inline void SystemManager::RegisterSystem(EntityComponentSystem* aECS)
	{
		static_assert(std::is_base_of_v<System<DerivedSystem>, DerivedSystem>, "Systems need to inherit from QPEcs::System");
		assert(!mySystems.contains(GetTypeName<DerivedSystem>()) && "System has already been registered!");
		mySystems[GetTypeName<DerivedSystem>()] = std::make_shared<DerivedSystem>();
		mySystems[GetTypeName<DerivedSystem>()]->myECS = aECS;
	}

	template <class DerivedSystem>
	inline void SystemManager::SetSignature(Signature aSignature)
	{
		static_assert(std::is_base_of_v<System<DerivedSystem>, DerivedSystem>, "Systems need to inherit from QPEcs::System");
		assert(mySystems.contains(GetTypeName<DerivedSystem>()) && "System needs to be registered before use!");

		mySignatures[GetTypeName<DerivedSystem>()] = aSignature;
	}

	template <class DerivedSystem>
	inline std::shared_ptr<DerivedSystem> SystemManager::GetSystem()
	{
		static_assert(std::is_base_of_v<System<DerivedSystem>, DerivedSystem>, "Systems need to inherit from QPEcs::SystemBase");
		assert(mySystems.contains(GetTypeName<DerivedSystem>()) && "System needs to be registered before use!");
		return std::static_pointer_cast<DerivedSystem>(mySystems[GetTypeName<DerivedSystem>()]);
	}

	template <class DerivedSystem>
	inline std::shared_ptr<DerivedSystem> SystemManager::GetAndRegisterSystem(EntityComponentSystem* aECS)
	{
		static_assert(std::is_base_of_v<System<DerivedSystem>, DerivedSystem>, "Systems need to inherit from QPEcs::SystemBase");

		if(!mySystems.contains(GetTypeName<DerivedSystem>()))
		{
			RegisterSystem<DerivedSystem>(aECS);
		}

		return GetSystem<DerivedSystem>();
	}

	template <class DerivedSystem>
	inline SystemManager::TypeName SystemManager::GetTypeName()
	{
		static_assert(std::is_base_of_v<System<DerivedSystem>, DerivedSystem>, "Systems need to inherit from QPEcs::SystemBase");
		return typeid(DerivedSystem).name();
	}
}
