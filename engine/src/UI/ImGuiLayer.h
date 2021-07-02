#pragma once

#include "Engine.h"

namespace fw
{
    class ImguiLayer
    {
    public:
        ImguiLayer(Engine& engine);
        ~ImguiLayer();

        void Begin();
        void End();

        void SetThemeColors();

    private:
        Engine& m_Engine;
    };
}  // namespace fw