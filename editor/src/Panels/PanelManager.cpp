#include "PanelManager.h"

#include <imgui/imgui.h>

namespace Wraith
{
    void PanelManager::OnUIRender()
    {
        ImGui::DockSpaceOverViewport();

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("View"))
            {
                for (auto& panel : m_Panels)
                    if (ImGui::MenuItem(panel->GetName(), nullptr, panel->IsVisible(), true))
                        panel->ToggleVisibility();
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        for (auto& panel : m_Panels)
            panel->OnUIRender();
    }
}  // namespace Wraith