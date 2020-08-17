#include "Engine.h"
#include "Core/Logger.h"
#include "Core/Common.h"
#include "Core/Filewatcher.h"
#include "Graphics/ContentManager.h"
#include "Graphics/TextureManager.h"

namespace fw
{
	Engine::Engine()
	{
		Logger::Create();
		Filewatcher::Create();
		ContentManager::Create();
		TextureManager::Create();
	}

	Engine::~Engine()
	{
		TextureManager::Destroy();
		ContentManager::Destroy();
		Filewatcher::Destroy();
		Logger::Destroy();
	}

	bool Engine::Init(Window* window)
	{
		m_Window = window;

		m_Window->Subscribe(Event::KeyPressed, [&](auto& e) {
			if (e.key.code == Key::Escape)
				m_Window->Close();
			});

		if (!m_Framework.Init(m_Window))
			return false;

		if (!m_RenderManager.Init(m_Window))
			return false;

		return true;
	}

	void Engine::Update(f32 dt, f32 total_time)
	{
		m_Framework.BeginFrame({ 0.2f,0.2f,0.2f,1 });

		m_RenderManager.Update(dt, total_time);
		m_RenderManager.Render();
		Filewatcher::Get()->FlushChanges();

		m_Framework.EndFrame();

		//Calculate average fps and display it on the window title
		{
			m_LastTimes.push_back(dt);

			if (m_LastTimes.size() > MaxNumTimesSaved)
				m_LastTimes.pop_front();

			f32 average_fps = [&]() {
				f32 accumulator = 0;

				for (auto& delta : m_LastTimes)
					accumulator += delta;

				return accumulator / m_LastTimes.size();
			}();

			m_Window->SetTitle("FPS: " + std::to_string((u32)(1.0f / average_fps)));
		}
	}

	void Engine::OnEvent(const Event& e)
	{
		m_RenderManager.OnEvent(e);
	}
}