#pragma once
#include "Graphics/OrthographicCameraController.h"
#include "Graphics/RenderManager.h"
#include "Window/Window.h"
#include "Core/Types.h"
#include <entt/entt.hpp>

namespace fw
{
	class Entity;
	class Scene
	{
	public:
		Scene();
		~Scene();

		bool Init(RenderManager* renderer);

		Entity CreateEntity(const std::string& name = std::string());

		void UpdateEditor(f32 dt, OrthographicCamera* editor_camera);
		void UpdateRuntime(f32 dt);

		void Play();
		void EndPlay();

	private:
		RenderManager* m_Renderer;
		bool m_IsPlaying;
		entt::registry m_Registry;

		friend class Entity;
		friend class SceneHierarchyPanel;
	};
}