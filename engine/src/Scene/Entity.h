#pragma once

#include <entt/entt.hpp>

#include "Core/Logger.h"
#include "Scene.h"

namespace fw
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene)
		    : m_Handle(handle)
		    , m_Scene(scene)
		{ }
		Entity(const Entity& other) = default;

		template <typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.all_of<T>(m_Handle);
		}

		template <typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			ASSERT_LOG(!HasComponent<T>(), "Entity already has component!");
			return m_Scene->m_Registry.emplace<T>(m_Handle, std::forward<Args>(args)...);
		}

		template <typename T>
		T& GetComponent()
		{
			ASSERT_LOG(HasComponent<T>(), "Entity doesn't have component!");
			return m_Scene->m_Registry.get<T>(m_Handle);
		}

		template <typename T>
		void RemoveComponent()
		{
			ASSERT_LOG(HasComponent<T>(), "Entity doesn't have component!");
			m_Scene->m_Registry.remove<T>(m_Handle);
		}

		operator bool() const
		{
			return m_Handle != entt::null;
		}
		operator uint32_t() const
		{
			return (uint32_t)m_Handle;
		}

		bool operator==(const Entity& other)
		{
			return m_Handle == other.m_Handle && m_Scene == other.m_Scene;
		}
		bool operator!=(const Entity& other)
		{
			return !(*this == other);
		}

	private:
		entt::entity m_Handle{ entt::null };
		Scene* m_Scene = nullptr;
	};
}  // namespace fw