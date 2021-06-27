#pragma once

#include "Entity.h"

namespace fw
{
	class ScriptableEntity
	{
	public:
		virtual ~ScriptableEntity()
		{ }

		template <typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}

	protected:
		virtual void OnCreate()
		{ }
		virtual void OnDestroy()
		{ }
		virtual void OnUpdate(f32 dt)
		{ }

		Input& GetInput()
		{
			return m_Input;
		}

	private:
		Entity m_Entity;
		Input m_Input;
		friend class Scene;
	};
}  // namespace fw