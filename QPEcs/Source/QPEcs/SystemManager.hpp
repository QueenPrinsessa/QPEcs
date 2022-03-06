#pragma once
#include "Component.h"
#include "Entity.h"
#include "SystemBase.h"
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
			template <class System>
			inline void RegisterSystem(EntityComponentSystem* aECS);

			template <class System>
			inline void SetSignature(Signature aSignature);

			inline void OnEntityDestroyed(Entity aEntity);

			inline void OnEntitySignatureChanged(Entity aEntity, Signature aEntitySignature);

			template <class System>
			inline std::shared_ptr<System> GetSystem();

			template <class System>
			inline std::shared_ptr<System> GetAndRegisterSystem(EntityComponentSystem* aECS);
		private:
			std::unordered_map<TypeName, Signature> mySignatures {};
			std::unordered_map<TypeName, std::shared_ptr<SystemBase>> mySystems {};

			template <class System>
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

	template <class System>
	inline void SystemManager::RegisterSystem(EntityComponentSystem* aECS)
	{
		static_assert(std::is_base_of_v<SystemBase, System>, "Systems need to inherit from QPEcs::SystemBase");
		assert(!mySystems.contains(GetTypeName<System>()) && "System has already been registered!");
		mySystems[GetTypeName<System>()] = std::make_shared<System>();
		mySystems[GetTypeName<System>()]->myECS = aECS;
	}

	template <class System>
	inline void SystemManager::SetSignature(Signature aSignature)
	{
		static_assert(std::is_base_of_v<SystemBase, System>, "Systems need to inherit from QPEcs::SystemBase");
		assert(mySystems.contains(GetTypeName<System>()) && "System needs to be registered before use!");

		mySignatures[GetTypeName<System>()] = aSignature;
	}

	template <class System>
	inline std::shared_ptr<System> SystemManager::GetSystem()
	{
		static_assert(std::is_base_of_v<SystemBase, System>, "Systems need to inherit from QPEcs::SystemBase");
		assert(mySystems.contains(GetTypeName<System>()) && "System needs to be registered before use!");
		return std::static_pointer_cast<System>(mySystems[GetTypeName<System>()]);
	}

	template <class System>
	inline std::shared_ptr<System> SystemManager::GetAndRegisterSystem(EntityComponentSystem* aECS)
	{
		static_assert(std::is_base_of_v<SystemBase, System>, "Systems need to inherit from QPEcs::SystemBase");

		if(!mySystems.contains(GetTypeName<System>()))
		{
			RegisterSystem<System>(aECS);
		}

		return GetSystem<System>();
	}

	template <class System>
	inline SystemManager::TypeName SystemManager::GetTypeName()
	{
		static_assert(std::is_base_of_v<SystemBase, System>, "Systems need to inherit from QPEcs::SystemBase");
		return typeid(System).name();
	}
}
