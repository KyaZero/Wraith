#pragma once

#include "Panel.h"

namespace Wraith
{
    class PanelManager
    {
    public:
        PanelManager() = default;
        ~PanelManager() = default;

        void OnUIRender();
        void AddPanel(Panel* panel)
        {
            m_Panels.push_back(panel);
        }

    private:
        std::vector<Panel*> m_Panels;
    };

}  // namespace Wraith