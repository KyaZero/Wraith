#pragma once

#include <Graphics/OrthographicCameraController.h>
#include <Graphics/RenderManager.h>

#include "Panels/Panel.h"

namespace Wraith
{
    class ViewportPanel : public Panel
    {
    public:
        ViewportPanel(RenderManager& render_manager, OrthographicCameraController& camera_controller)
            : m_RenderManager(render_manager)
            , m_CameraController(camera_controller)
        { }
        virtual ~ViewportPanel() = default;

        void OnUIRender() override;
        const char* GetName() const override { return "Viewport"; }

    private:
        Vec2f m_ViewportBounds[2];
        Vec2f m_ViewportSize;

        RenderManager& m_RenderManager;
        OrthographicCameraController& m_CameraController;
    };
}  // namespace Wraith