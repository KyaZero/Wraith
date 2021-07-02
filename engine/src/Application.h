#pragma once

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

        ImguiLayer& GetImguiLayer()
        {
            return *m_ImguiLayer;
        }

    protected:
        std::unique_ptr<Window> m_Window;
        std::unique_ptr<Engine> m_Engine;
        std::unique_ptr<ImguiLayer> m_ImguiLayer;

        Timer m_Timer;
        std::string m_Name;
    };

    Application* CreateApplication();
}  // namespace fw