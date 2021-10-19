#pragma once

#include "Engine.h"
#include "Window/Window.h"

int main(int argc, char** argv);

namespace Wraith
{
    class Application
    {
    public:
        Application(const std::string& name = "Application");
        virtual ~Application();

        bool Run();

        virtual void OnUpdate(f32 dt)
        { }
        virtual void OnUIRender()
        { }

    protected:
        std::unique_ptr<Window> m_Window;
        std::unique_ptr<Engine> m_Engine;
        std::string m_Name;
    };
}  // namespace Wraith