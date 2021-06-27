#include "Engine.h"

#include "Core/Common.h"
#include "Core/Filewatcher.h"
#include "Core/Logger.h"
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

	bool Engine::Init(std::shared_ptr<Window> window)
	{
		m_Window = window;

		if (!m_Framework.Init(m_Window))
			return false;

		if (!m_RenderManager.Init(m_Window))
			return false;

		return true;
	}

	void Engine::BeginFrame()
	{
		m_Framework.BeginFrame({ 0.2f, 0.2f, 0.2f, 1 });
	}

	void Engine::EndFrame()
	{
		m_Framework.EndFrame();
	}

	void Engine::Update(f32 dt, f32 total_time)
	{
		m_RenderManager.Render(dt, total_time);
		m_Framework.SetBackbufferAsActiveTarget();
		Filewatcher::Get()->FlushChanges();
		Input::FlushState();

		// Calculate average fps and display it on the window title
		{
			static auto original_title = m_Window->GetTitle();

			m_LastTimes.push_back(dt);

			if (m_LastTimes.size() > MaxNumTimesSaved)
				m_LastTimes.pop_front();

			f32 average_fps = [&]() {
				f32 accumulator = 0;

				for (auto& delta : m_LastTimes)
					accumulator += delta;

				return accumulator / m_LastTimes.size();
			}();

			m_Window->SetTitle(original_title + " - FPS: " + std::to_string((u32)(1.0f / average_fps)));
		}
	}
}  // namespace fw