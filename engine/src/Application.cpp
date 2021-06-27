#include "Application.h"
#include <imgui.h>

namespace fw
{
	Application::Application(const std::string& name) : m_Window(), m_Name(name)
	{
		m_Window = std::make_shared<Window>(Vec2u(1600, 900), m_Name);
		m_Engine = std::make_shared<Engine>();

		if (!m_Engine->Init(m_Window))
		{
			ASSERT_LOG(false, "Failed To Initialize Engine.");
		}

		m_ImguiLayer = std::make_shared<ImguiLayer>(m_Engine);
	}

	bool Application::Run()
	{
		while (!m_Window->ShouldClose())
		{
			m_Timer.Update();

			m_Engine->BeginFrame();

			{
				OnUpdate(m_Timer.GetDeltaTime());
				m_Engine->Update(m_Timer.GetDeltaTime(), m_Timer.GetTotalTime());
			}

			{
				m_ImguiLayer->Begin();
				OnUIRender();
				m_ImguiLayer->End();
			}

			m_Engine->EndFrame();

			// Window events such as resizing, input, etc..
			m_Window->PollEvents();
		}
		return true;
	}
}