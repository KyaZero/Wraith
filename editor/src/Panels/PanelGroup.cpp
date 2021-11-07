#include "PanelGroup.h"

#include <imgui/imgui.h>

namespace Wraith
{
    void PanelGroup::RenderWindows()
    {
        for (auto& panel : m_Panels)
        {
            panel->OnUIRender();
        }
        for (auto& panel_group : m_PanelGroups)
        {
            panel_group->RenderWindows();
        }
    }
    void PanelGroup::RenderMenus()
    {
        if (ImGui::BeginMenu(m_Name.c_str()))
        {
            for (auto& panel : m_Panels)
            {
                if (ImGui::MenuItem(panel->GetName(), nullptr, panel->IsVisible(), true))
                    panel->ToggleVisibility();
            }
            for (auto& panel_group : m_PanelGroups)
            {
                panel_group->RenderMenus();
            }
            if (!m_Panels.empty())
            {
                ImGui::Separator();
                if (ImGui::MenuItem(std::format("Show {} Group", m_Name).c_str()))
                {
                    for (auto& panel : m_Panels)
                    {
                        panel->SetVisibility(true);
                    }
                }
                if (ImGui::MenuItem(std::format("Hide {} Group", m_Name).c_str()))
                {
                    for (auto& panel : m_Panels)
                    {
                        panel->SetVisibility(false);
                    }
                }
            }
            ImGui::EndMenu();
        }
    }
}  // namespace Wraith