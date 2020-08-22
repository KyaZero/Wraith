#include "App.h"
#include "Window/Input.h"

namespace fw
{
	App::App() : m_Window()
	{
	}

	App::~App()
	{
		delete m_Window;
	}

	bool App::Start()
	{
		m_Window = new Window(VideoMode(1280, 720), L"App", Style::Default);
		m_Window->Subscribe(Event::Closed, [&](auto e) { 
			m_Window->Close(); 
		});
		//m_Window->SetCursorVisible(false);

		if (!m_Engine.Init(m_Window))
			return false;

		return true;
	}

	bool App::Run()
	{
		while (m_Window->IsOpen())
		{
			m_Timer.Update();
			m_Engine.Update(m_Timer.GetDeltaTime(), m_Timer.GetTotalTime());
			
			Event event;
			while (m_Window->PollEvent(event))
			{
				m_Engine.OnEvent(event);
			}
		}

		return true;
	}
}