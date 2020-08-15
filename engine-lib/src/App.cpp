#include "App.h"
#include "Window/Input.h"
#include "Core/Logger.h"
#include "Core/Common.h"

namespace fw
{
	App::App() : m_Window()
	{
		Logger::Create();
	}

	App::~App()
	{
		delete m_Window;

		Logger::Destroy();
	}

	bool App::Start()
	{
		m_Window = new Window(VideoMode(800, 600), L"App", Style::Default);

		m_Window->Subscribe(Event::KeyPressed, [&](auto& e) {
			if (e.key.code == Key::Escape)
				m_Window->Close();
		});

		m_Framework.Init(m_Window);

		//m_Window->SetCursorVisible(false);
		//m_Window->SetCursorGrabbed(true);

		return true;
	}

	bool App::Run()
	{
		while (m_Window->IsOpen())
		{
			static f32 x, y;
			Event event;
			while (m_Window->PollEvent(event))
			{
				if (event.type == Event::Closed)
					m_Window->Close();

				if (event.type == Event::MouseMove)
				{
					x = event.mouse_move.x;
					y = event.mouse_move.y;
				}
			}

			auto w = (f32)m_Window->GetSize().x;
			auto h = (f32)m_Window->GetSize().y;
			m_Framework.BeginFrame({x / w,((x / w) + (y / h)) / 2.0f,y / h,1});

			m_Framework.EndFrame();
		}

		return true;
	}
}