#pragma once

#include "Graphics/Framework.h"
#include "Window/Window.h"

namespace Wraith
{
    class ImguiLayer
    {
    public:
        ImguiLayer(Framework& engine, Window& window);
        ~ImguiLayer();

        void Begin();
        void End();

        void LoadSettings(std::filesystem::path settings_file)
        {
            m_SettingsToLoad = settings_file;
        }

    private:
        void SetThemeColors();

        Framework& m_Framework;
        Window& m_Window;
        std::optional<std::filesystem::path> m_SettingsToLoad = std::nullopt;
    };
}  // namespace Wraith