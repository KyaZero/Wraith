#pragma once
#include "Graphics/OrthographicCameraController.h"
#include "Graphics/RenderManager.h"
#include "Window/Window.h"
#include "Window/Event.h"
#include "Core/Types.h"
#include "entt.hpp"

namespace fw
{
	class Entity;
	class Scene
	{
	public:
		Scene();
		~Scene();

		bool Init(Window* window, RenderManager* renderer);

		Entity CreateEntity(const std::string& name = std::string());

		void OnEvent(const Event& e);
		void Update(f32 dt, f32 total_time);

	private:
		RenderManager* m_Renderer;
		Window* m_Window;

		OrthographicCameraController m_Camera;
		entt::registry m_Registry;
		friend class Entity;
	};
}