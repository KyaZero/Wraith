#pragma once

#include "Graphics/Framework.h"
#include "Window/Window.h"

namespace fw
{
    class ImguiLayer
    {
    public:
        ImguiLayer(Framework& engine, Window& window);
        ~ImguiLayer();

        void Begin();
        void End();

        void SetThemeColors();

    private:
        Framework& m_Framework;
        Window& m_Window;
    };
}  // namespace fw