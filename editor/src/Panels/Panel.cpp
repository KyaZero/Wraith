#include "Panel.h"

#include <imgui/imgui.h>

namespace Wraith
{
    bool Panel::Begin(int flags)
    {
        m_ShouldCallEnd = m_IsVisible;
        if (!m_IsVisible)
            return false;

        bool visible = ImGui::Begin(GetName(), &m_IsVisible, flags);

        if (visible)
        {
            m_IsFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows);
            m_IsHovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows);
        }

        return visible;
    }

    void Panel::End()
    {
        if (m_ShouldCallEnd)
            ImGui::End();
    }

}  // namespace Wraith