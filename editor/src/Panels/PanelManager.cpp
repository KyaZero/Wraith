#include "PanelManager.h"

#include <imgui/imgui.h>
#include <shellapi.h>
#include <tinyfd/tinyfiledialogs.h>

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
                {
                    if (ImGui::MenuItem(panel->GetName(), nullptr, panel->IsVisible(), true))
                        panel->ToggleVisibility();
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Help"))
            {
                if (ImGui::MenuItem("Open Temp Directory"))
                {
                    ShellExecuteW(0, L"open", TEMP_DIRECTORY.native().c_str(), 0, 0, SW_SHOWDEFAULT);
                }
                if (ImGui::MenuItem("Choose Layout"))
                {
                    const char* filter = "*.*";
                    auto path = tinyfd_openFileDialog("ImGui Layout", "", 1, &filter, nullptr, 0);
                    if (path)
                        m_ImguiLayer.LoadSettings(path);
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        for (auto& panel : m_Panels)
        {
            panel->OnUIRender();
        }
        ImGui::ShowDemoWindow();
    }
}  // namespace Wraith