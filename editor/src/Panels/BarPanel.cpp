#include "BarPanel.h"

#include <imgui/imgui.h>

namespace Wraith
{
    void BarPanel::OnUIRender()
    {
        if (Begin(ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoTitleBar))
        {
            if (ImGui::Button(m_IsPlay ? "Play" : "End Play"))
            {
                if (m_IsPlay)
                    m_OnBeginPlay();
                else
                    m_OnEndPlay();

                m_IsPlay = !m_IsPlay;
            }
        }
        End();
    }
}  // namespace Wraith