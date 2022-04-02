#include "ViewportPanel.h"

#include <Graphics/Texture.h>
#include <Input/Input.h>
#include <Scene/Components.h>
#include <imgui/imgui.h>

#include "ImGuizmo/ImGuizmo.h"

namespace Wraith
{
    void ViewportPanel::OnUIRender()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
        if (Begin())
        {
            ImGuizmo::SetDrawlist();
            ImGuizmo::SetRect(
                m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

            ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

            auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
            auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
            auto viewportOffset = ImGui::GetWindowPos();
            m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
            m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

            if (m_ViewportSize.x != viewportPanelSize.x || m_ViewportSize.y != viewportPanelSize.y)
            {
                m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

                m_RenderManager.Resize(m_ViewportSize.x, m_ViewportSize.y);
                m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
                // m_ActiveScene->Resize(m_ViewportSize.x, m_ViewportSize.y);
            }

            Input::BlockUIEvents(!IsFocused() && !IsHovered());

            // Texture& texture = m_RenderManager.GetRenderTexture();
            // Draw first
            // ImGui::Image(texture.GetShaderResourceView(),
            //             ImVec2{ m_ViewportSize.x, m_ViewportSize.y },
            //             ImVec2{ 0, 0 },
            //             ImVec2{ 1, 1 });
        }
        End();

        ImGui::PopStyleVar();
    }
}  // namespace Wraith