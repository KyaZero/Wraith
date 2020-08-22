#pragma once
#include "Scene.h"
#include "entt.hpp"

namespace fw
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene) : m_Handle(handle), m_Scene(scene) { }
		Entity(const Entity& other) = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			ASSERT_LOG(!HasComponent<T>(), "Entity already has component!");
			return m_Scene->m_Registry.emplace<T>(m_Handle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			ASSERT_LOG(HasComponent<T>(), "Entity doesn't have component!");
			return m_Scene->m_Registry.get<T>(m_Handle);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.has<T>(m_Handle);
		}

		template<typename T>
		void RemoveComponent()
		{
			ASSERT_LOG(HasComponent<T>(), "Entity doesn't have component!");
			m_Scene->m_Registry.remove<T>(m_Handle);
		}

		operator bool() const { return m_Handle != entt::null; }

	private:
		entt::entity m_Handle{ entt::null };
		Scene* m_Scene = nullptr;
	};
}