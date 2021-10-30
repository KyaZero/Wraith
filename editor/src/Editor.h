#pragma once

#include <memory>

#include <Application.h>
#include <Scene/Entity.h>
#include <Scene/Scene.h>

#include "Panels/BarPanel.h"
#include "Panels/PanelManager.h"
#include "Panels/PropertiesPanel.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ViewportPanel.h"

namespace Wraith
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
        std::unique_ptr<Scene> m_ActiveScene;
        OrthographicCameraController m_CameraController;

        Entity m_CameraEntity;

        bool m_IsScenePlaying = false;

        PanelManager m_PanelManager;
        std::unique_ptr<ViewportPanel> m_ViewportPanel;
        std::unique_ptr<BarPanel> m_BarPanel;
        std::unique_ptr<SceneHierarchyPanel> m_SceneHierarchyPanel;
        std::unique_ptr<PropertiesPanel> m_PropertiesPanel;
    };
}  // namespace Wraith