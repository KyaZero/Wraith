#include "BarPanel.h"

#include <imgui/imgui.h>

#include "Editor.h"
#include "Graphics/TextureManager.h"

namespace Wraith
{
    void BarPanel::OnUIRender()
    {
        if (Begin(ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar |
                  ImGuiWindowFlags_NoScrollWithMouse))
        {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
            ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
            auto& colors = ImGui::GetStyle().Colors;
            const auto& button_hovered = colors[ImGuiCol_ButtonHovered];
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                                  ImVec4(button_hovered.x, button_hovered.y, button_hovered.z, 0.5f));
            const auto& button_active = colors[ImGuiCol_ButtonActive];
            ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                                  ImVec4(button_active.x, button_active.y, button_active.z, 0.5f));

            ImVec2 size = ImVec2(ImGui::GetWindowHeight() - 16.0f, ImGui::GetWindowHeight() - 16.0f);
            const Texture& tex = m_IsPlay ? TextureManager::Get()->GetTexture(m_PlayButton)
                                          : TextureManager::Get()->GetTexture(m_PauseButton);

            if (ImGui::ImageButton(tex.GetShaderResourceView(), size, ImVec2(0, 0), ImVec2(1, 1), 0))
            {
                if (m_IsPlay)
                    m_OnBeginPlay();
                else
                    m_OnEndPlay();

                m_IsPlay = !m_IsPlay;
            }

            ImGui::SameLine();
            if (ImGui::ImageButton(TextureManager::Get()->GetTexture(m_TranslateButton).GetShaderResourceView(), size))
            {
                m_Editor.SetGizmoOperation(ImGuizmo::OPERATION::TRANSLATE);
            }

            ImGui::SameLine();
            if (ImGui::ImageButton(TextureManager::Get()->GetTexture(m_RotateButton).GetShaderResourceView(), size))
            {
                m_Editor.SetGizmoOperation(ImGuizmo::OPERATION::ROTATE);
            }

            ImGui::SameLine();
            if (ImGui::ImageButton(TextureManager::Get()->GetTexture(m_ScaleButton).GetShaderResourceView(), size))
            {
                m_Editor.SetGizmoOperation(ImGuizmo::OPERATION::SCALE);
            }

            ImGui::SameLine();
            if (ImGui::Button(m_Editor.GetGizmoSpace() == ImGuizmo::MODE::WORLD ? "World" : "Local"))
                m_Editor.SetGizmoSpace(m_Editor.GetGizmoSpace() == ImGuizmo::MODE::WORLD ? ImGuizmo::MODE::LOCAL
                                                                                         : ImGuizmo::MODE::WORLD);

            if (m_Editor.GetEditorCamera().GetCameraState() != CameraState::FreeCam && !ImGuizmo::IsUsing())
            {
                if (m_Input.IsPressed(Key::W))
                {
                    m_Editor.SetGizmoOperation(ImGuizmo::OPERATION::TRANSLATE);
                }
                if (m_Input.IsPressed(Key::E))
                {
                    m_Editor.SetGizmoOperation(ImGuizmo::OPERATION::ROTATE);
                }
                if (m_Input.IsPressed(Key::R))
                {
                    m_Editor.SetGizmoOperation(ImGuizmo::OPERATION::SCALE);
                }
            }
        }
        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor(3);
        End();
    }
}  // namespace Wraith