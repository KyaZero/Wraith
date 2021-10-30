#pragma once

#include <imgui/imgui.h>

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

        void LoadSettings(std::filesystem::path settings_file);
        void OnContentScale(f32 scale);

    private:
        void SetStyle();
        void SetScales();

        Framework& m_Framework;
        Window& m_Window;
        std::optional<std::filesystem::path> m_SettingsToLoad = std::nullopt;

        struct FontData
        {
            f32 scale;
            ImFont* font;
        };
        std::vector<FontData> m_Fonts;
        f32 m_ContentScale = 1.f;
    };
}  // namespace Wraith