#pragma once

#include "Application.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Scene/Entity.h"
#include "Scene/Scene.h"

namespace fw
{
    class Editor : public Application
    {
    public:
        Editor();
        ~Editor() = default;

        void OnUpdate(f32 dt) override;
        void OnUIRender() override;

        void OnScenePlay();
        void OnSceneEndPlay();

    private:
        std::shared_ptr<Scene> m_ActiveScene;
        OrthographicCameraController m_CameraController;
        Vec2f m_ViewportBounds[2];
        Vec2f m_ViewportSize;

        Entity m_CameraEntity;

        bool m_ViewportFocused = false;
        bool m_ViewportHovered = false;
        bool m_IsScenePlaying = false;
        bool m_IsPlay = true;

        SceneHierarchyPanel m_SceneHierarchyPanel;
    };
}  // namespace fw