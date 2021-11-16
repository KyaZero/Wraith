#include "BarPanel.h"

#include <imgui/imgui.h>

#include "Editor.h"

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
            ImGui::SameLine();
            if (ImGui::Button("Translate"))
                m_Editor.SetGizmoOperation(ImGuizmo::OPERATION::TRANSLATE);
            ImGui::SameLine();
            if (ImGui::Button("Rotate"))
                m_Editor.SetGizmoOperation(ImGuizmo::OPERATION::ROTATE);
            ImGui::SameLine();
            if (ImGui::Button("Scale"))
                m_Editor.SetGizmoOperation(ImGuizmo::OPERATION::SCALE);
            ImGui::SameLine();
            if (ImGui::Button("World Space"))
                m_Editor.SetGizmoSpace(ImGuizmo::MODE::WORLD);
            ImGui::SameLine();
            if (ImGui::Button("Local Space"))
                m_Editor.SetGizmoSpace(ImGuizmo::MODE::LOCAL);

            
            // Check for rmb up, since otherwise we're in free cam and move with wasd
            if (m_Editor.GetEditorCamera().GetCameraState() != CameraState::FreeCam)
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
        End();
    }
}  // namespace Wraith