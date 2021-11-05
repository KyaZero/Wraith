#pragma once

#include <functional>

#include "Panel.h"
#include "Scene/Entity.h"
#include "Scene/Scene.h"

namespace Wraith
{
    class SceneHierarchyPanel : public Panel
    {
        using Handler = std::function<void(Entity)>;

    public:
        SceneHierarchyPanel(Scene& context, Handler on_entity_selected)
            : m_Context(context)
            , m_OnEntitySelected(on_entity_selected)
        { }
        virtual ~SceneHierarchyPanel() = default;

        void OnUIRender() override;
        const char* GetName() const override { return "Scene Hierarchy"; }

    private:
        void DrawEntityNode(Entity entity);

        void SelectEntity(Entity entity);

        Scene& m_Context;
        Entity m_SelectedEntity;
        Handler m_OnEntitySelected;
    };
}  // namespace Wraith