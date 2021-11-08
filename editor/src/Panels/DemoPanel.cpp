#include "DemoPanel.h"

#include <imgui/imgui.h>

namespace Wraith
{
    void DemoPanel::OnUIRender()
    {
        if (IsVisible())
            ImGui::ShowDemoWindow(&m_IsVisible);
    }

}  // namespace Wraith
