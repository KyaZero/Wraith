#pragma once

#include <UI/ImGuiLayer.h>

#include "Panel.h"

namespace Wraith
{
    class PanelManager
    {
    public:
        PanelManager(ImguiLayer& imgui_layer)
            : m_ImguiLayer(imgui_layer)
        { }
        ~PanelManager() = default;

        void OnUIRender();
        void AddPanel(Panel* panel)
        {
            m_Panels.push_back(panel);
        }

    private:
        std::vector<Panel*> m_Panels;
        ImguiLayer& m_ImguiLayer;
    };

}  // namespace Wraith