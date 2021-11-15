#include "ViewportPanel.h"

#include <Graphics/Texture.h>
#include <Input/Input.h>
#include <imgui/imgui.h>

namespace Wraith
{
    void ViewportPanel::OnUIRender()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
        if (Begin())
        {
            ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

            if (m_ViewportSize.x != viewportPanelSize.x || m_ViewportSize.y != viewportPanelSize.y)
            {
                m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

                m_RenderManager.Resize(m_ViewportSize.x, m_ViewportSize.y);
                m_CameraController.Resize(m_ViewportSize.x, m_ViewportSize.y);
                // m_ActiveScene->Resize(m_ViewportSize.x, m_ViewportSize.y);
            }

            Input::BlockUIEvents(!IsFocused() && !IsHovered());

            Texture& texture = m_RenderManager.GetRenderTexture();
            // Draw first
            ImGui::Image(texture.GetShaderResourceView(),
                         ImVec2{ m_ViewportSize.x, m_ViewportSize.y },
                         ImVec2{ 0, 0 },
                         ImVec2{ 1, 1 });
        }
        End();

        ImGui::PopStyleVar();
    }
}  // namespace Wraith