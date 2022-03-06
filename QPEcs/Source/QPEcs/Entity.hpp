#pragma once
#include "Types.h"
#include "EntityComponentSystem.hpp"

namespace QPEcs
{
	//using Entity = EntityType;

	constexpr EntityType MaxEntities = 5000;
	constexpr EntityType NullEntity = 0;

	using Entity = EntityType;

<<<<<<< HEAD
=======
	//class EntityComponentSystem;
	//class Entity
	//{
	//	friend class EntityComponentSystem;
	//	public:
	//		Entity(EntityType aEntity);
	//		Entity(EntityType aEntity, EntityComponentSystem* aECS);
	//		Entity(const Entity&) = default;
	//		~Entity() = default;

	//		operator EntityType();

	//		bool operator==(const Entity& aOther);
	//		bool operator!=(const Entity& aOther);


	//		template <class Component>
	//		inline bool HasComponent();

	//		template <class Component>
	//		inline void AddComponent(Component aComponent);

	//		template <class Component>
	//		inline void RemoveComponent();

	//		template <class Component>
	//		inline Component& GetComponent();

	//		template <class Component>
	//		inline Component& GetOrAddComponent();

	//	private:
	//		EntityType myEntity { NullEntity };
	//		EntityComponentSystem* myECS { nullptr};
	//};

	//template <class Component>
	//inline bool Entity::HasComponent()
	//{
	//	return myECS->HasComponent<Component>(*this);
	//}

	//template <class Component>
	//inline void Entity::AddComponent(Component aComponent)
	//{
	//	myECS->AddAndRegisterComponent<Component>(*this, aComponent);
	//}

	//template <class Component>
	//void Entity::RemoveComponent()
	//{
	//	myECS->RemoveComponent<Component>(*this);
	//}

	//template <class Component>
	//Component& Entity::GetComponent()
	//{
	//	return myECS->GetComponent<Component>(*this);
	//}

	//template <class Component>
	//Component& Entity::GetOrAddComponent()
	//{
	//	return myECS->GetComponent<Component>(*this);
	//}

	//inline Entity::Entity(EntityType aEntity)
	//{
	//}

	//inline Entity::Entity(EntityType aEntity, EntityComponentSystem* aECS)
	//	: Entity(aEntity)
	//{
	//}

	//inline Entity::operator EntityType()
	//{
	//	return myEntity;
	//}

	//inline bool Entity::operator==(const Entity& aOther)
	//{
	//	return myEntity == aOther.myEntity; // check signature too?
	//}

	//inline bool Entity::operator!=(const Entity& aOther)
	//{
	//	return !operator==(aOther);
	//}
>>>>>>> e4ae228104eabef27393d8f9cb31a944654ad077
}
