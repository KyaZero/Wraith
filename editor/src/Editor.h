#pragma once

#include <memory>

#include "Panels/PanelGroup.h"
#include "Panels/ViewportPanel.h"
#include "Wraith.h"

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

    private:
        std::unique_ptr<Scene> m_ActiveScene;
        OrthographicCameraController m_CameraController;

        Entity m_CameraEntity;

        bool m_IsScenePlaying = false;

        PanelGroup m_PanelManager;
        ViewportPanel* m_ViewportPanel;
    };
}  // namespace Wraith