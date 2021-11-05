#include "Application.h"

namespace Wraith
{
    Application::Application(const std::string& name)
        : m_Window()
        , m_Name(name)
    {
        m_Window = std::make_unique<Window>(Vec2u(1600, 900), m_Name);
        m_Engine = std::make_unique<Engine>(*m_Window);

        if (!m_Engine->Init([this]() { OnUIRender(); }, [this](f32 dt) { OnUpdate(dt); }))
        {
            ASSERT_LOG(false, "Failed To Initialize Engine.");
        }
    }

    Application::~Application() { }

    bool Application::Run()
    {
        while (!m_Window->ShouldClose())
        {
            m_Engine->BeginFrame();
            m_Engine->Update();
            m_Engine->EndFrame();

            // Window events such as resizing, input, etc..
            m_Window->PollEvents();
        }
        return true;
    }
}  // namespace Wraith
