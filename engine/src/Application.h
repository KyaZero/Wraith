#pragma once

#include <memory>

#include "Core/Timer.h"
#include "Engine.h"
#include "UI/ImguiLayer.h"
#include "Window/Window.h"

int main(int argc, char** argv);

namespace fw
{
    class Application
    {
    public:
        Application(const std::string& name = "Application");
        virtual ~Application() = default;

        bool Run();

        virtual void OnUpdate(f32 dt)
        { }
        virtual void OnUIRender()
        { }

        std::shared_ptr<ImguiLayer> GetImguiLayer()
        {
            return m_ImguiLayer;
        }

    protected:
        std::shared_ptr<Window> m_Window;
        std::shared_ptr<Engine> m_Engine;
        std::shared_ptr<ImguiLayer> m_ImguiLayer;

        Timer m_Timer;
        std::string m_Name;
    };

    Application* CreateApplication();
}  // namespace fw