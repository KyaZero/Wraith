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

                ImGui::Separator();

                if (ImGui::BeginMenu("Editor Layout"))
                {
                    if (ImGui::MenuItem("Save Layout"))
                    {
                        const char* filter = "*.ini";
                        auto path = tinyfd_saveFileDialog("ImGui Layout", "", 1, &filter, nullptr);
                        if (path)
                            ImGui::SaveIniSettingsToDisk(path);
                    }
                    if (ImGui::MenuItem("Load Layout"))
                    {
                        const char* filter = "*.ini";
                        auto path = tinyfd_openFileDialog("Load Layout", "", 1, &filter, nullptr, 0);
                        if (path)
                            m_ImguiLayer.LoadSettings(path);
                    }
                    ImGui::EndMenu();
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Help"))
            {
                if (ImGui::MenuItem("Open Temp Directory"))
                {
                    ShellExecuteW(0, L"open", TEMP_DIRECTORY.native().c_str(), 0, 0, SW_SHOWDEFAULT);
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