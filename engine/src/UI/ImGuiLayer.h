#pragma once

// ImGuizmo is dependent on imgui being included before it.
// clang-format off
#include <imgui/imgui.h>
#include <ImGuizmo/ImGuizmo.h>
// clang-format on

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

        Framework& m_Framework;
        Window& m_Window;
        std::optional<std::filesystem::path> m_SettingsToLoad = std::nullopt;

        struct FontData
        {
            f32 scale;
            ImFont* font;
        };
        std::vector<FontData> m_Fonts;
        ImGuiStyle m_Style;
    };
}  // namespace Wraith