#include "App.h"
#include "Window/Input.h"
#include "Core/Logger.h"
#include "Core/Common.h"
#include "Core/Filewatcher.h"

namespace fw
{
	App::App() : m_Window()
	{
		Logger::Create();
		Filewatcher::Create();
	}

	App::~App()
	{
		delete m_Window;

		Filewatcher::Destroy();
		Logger::Destroy();
	}

	bool App::Start()
	{
		m_Window = new Window(VideoMode(1280, 720), L"App", Style::Default);

		m_Window->Subscribe(Event::KeyPressed, [&](auto& e) {
			if (e.key.code == Key::Escape)
				m_Window->Close();
		});

		if (!m_Framework.Init(m_Window))
			return false;

		if (!m_RenderManager.Init(m_Window))
			return false;

		//m_Window->SetCursorVisible(false);
		//m_Window->SetCursorGrabbed(true);

		return true;
	}

	bool App::Run()
	{
		while (m_Window->IsOpen())
		{
			m_Timer.Update();
			//Calculate average fps and display it on the window title
			{
				m_LastTimes.push_back(m_Timer.GetDeltaTime());

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

			Event event;
			while (m_Window->PollEvent(event))
			{
				if (event.type == Event::Closed)
					m_Window->Close();
			}

			m_Framework.BeginFrame({0.2f,0.2f,0.2f,1});
			m_RenderManager.Render(m_Timer.GetDeltaTime());
			Filewatcher::Get()->FlushChanges();
			m_Framework.EndFrame();
		}

		return true;
	}
}