#pragma once
#include "GenericView.hpp"
namespace QPEcs
{
	template <class ... Components>
	class View;

	class EntityComponentSystem;
	class ViewManager
	{
		using TypeName = const char*;
	public:
		ViewManager() = delete;
		explicit ViewManager(ComponentManager* aComponentManager);

		template <class ... Components>
		inline void RegisterView(EntityComponentSystem* aECS);

		template <class ... Components>
		inline bool IsRegistered();

		inline void OnEntityDestroyed(Entity aEntity);

		inline void OnEntitySignatureChanged(Entity aEntity, Signature aEntitySignature);

		template <class ... Components>
		inline std::shared_ptr<View<Components...>> GetView();
	private:
		ComponentManager* myComponentManager { nullptr };
		std::unordered_map<TypeName, std::shared_ptr<GenericView>> myViews {};
		std::unordered_map<TypeName, Signature> myViewSignatures {};

		template <class ... Components>
		inline TypeName GetTypeName();
	};

	template <class ... Components>
	void ViewManager::RegisterView(EntityComponentSystem* aECS)
	{
		assert(!myViews.contains(GetTypeName<View<Components...>>()) && "View has already been registered!");
		myViews[GetTypeName<View<Components...>>()] = std::make_shared<View<Components...>>();
		myViews[GetTypeName<View<Components...>>()]->myECS = aECS;

		Signature signature;
		((signature.set(myComponentManager->GetComponentType<Components>())), ...);
		myViewSignatures[GetTypeName<View<Components...>>()] = signature;
	}

	template <class ... Components>
	bool ViewManager::IsRegistered()
	{
		return myViews.contains(GetTypeName<View<Components...>>());
	}

	template <class ... Components>
	std::shared_ptr<View<Components...>> ViewManager::GetView()
	{
		assert(myViews.contains(GetTypeName<View<Components...>>()) && "View hasn't been registered!");
		return std::static_pointer_cast<View<Components...>>(myViews[GetTypeName<View<Components...>>()]);
	}

	template <class ... Components>
	ViewManager::TypeName ViewManager::GetTypeName()
	{
		return typeid(View<Components...>).name();
	}

	inline ViewManager::ViewManager(ComponentManager* aComponentManager)
		: myComponentManager(aComponentManager)
	{
	}

	inline void ViewManager::OnEntityDestroyed(Entity aEntity)
	{
		for (auto& [typeName, view] : myViews)
		{
			view->myEntities.erase(aEntity);
		}
	}

	inline void ViewManager::OnEntitySignatureChanged(Entity aEntity, Signature aEntitySignature)
	{
		for (auto const& [typeName, view] : myViews)
		{
			const auto& viewSignature = myViewSignatures[typeName];

			if ((aEntitySignature & viewSignature) == viewSignature)
			{
				view->myEntities.insert(aEntity);
			}
			else
			{
				view->myEntities.erase(aEntity);
			}
		}
	}
}
