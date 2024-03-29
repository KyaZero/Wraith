#pragma once

#include <memory>

#include <Scene/EditorCamera.h>
#include <Wraith.h>

#include "Panels/PanelGroup.h"
#include "Panels/Play/ViewportPanel.h"

namespace Wraith
{
    class Editor : public Application
    {
    public:
        Editor();
        ~Editor();

        void OnUpdate(f32 dt) override;
        void OnUIRender() override;

        void OnScenePlay();
        void OnSceneEndPlay();

        void SetGizmoOperation(ImGuizmo::OPERATION op) { m_GizmoOperation = op; }
        void SetGizmoSpace(ImGuizmo::MODE space) { m_OperationSpace = space; }

        ImGuizmo::OPERATION GetGizmoOperation() const { return m_GizmoOperation; }
        ImGuizmo::MODE GetGizmoSpace() const { return m_OperationSpace; }

        const EditorCamera& GetEditorCamera() const;

    private:
        std::unique_ptr<Scene> m_ActiveScene;
        EditorCamera m_EditorCamera;

        Entity m_CameraEntity;

        bool m_IsScenePlaying = false;

        PanelGroup m_PanelManager;
        ViewportPanel* m_ViewportPanel;
        Entity m_SelectedEntity;
        Input m_Input;

        ImGuizmo::OPERATION m_GizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
        ImGuizmo::MODE m_OperationSpace = ImGuizmo::MODE::LOCAL;
    };
}  // namespace Wraith