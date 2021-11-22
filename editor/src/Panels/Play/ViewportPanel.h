#pragma once

#include <Graphics/RenderManager.h>

#include "Panels/Panel.h"
#include "Scene/EditorCamera.h"

namespace Wraith
{
    class ViewportPanel : public Panel
    {
    public:
        ViewportPanel(RenderManager& render_manager, EditorCamera& editor_camera)
            : m_RenderManager(render_manager)
            , m_EditorCamera(editor_camera)
        { }
        virtual ~ViewportPanel() = default;

        void OnUIRender() override;
        const char* GetName() const override { return "Viewport"; }

    private:
        Vec2f m_ViewportBounds[2];
        Vec2f m_ViewportSize;

        RenderManager& m_RenderManager;
        EditorCamera& m_EditorCamera;
    };
}  // namespace Wraith