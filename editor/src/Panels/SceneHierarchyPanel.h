#pragma once

#include "Scene/Entity.h"
#include "Scene/Scene.h"

namespace fw
{
    class SceneHierarchyPanel
    {
    public:
        SceneHierarchyPanel() = default;
        SceneHierarchyPanel(std::shared_ptr<Scene> context);

        void SetContext(std::shared_ptr<Scene> context);

        void OnUIRender();

    private:
        void DrawEntityNode(Entity entity);

        // Move this stuff to PropertiesPanel
        void DrawComponents(Entity entity);

        std::shared_ptr<Scene> m_Context;
        Entity m_SelectionContext;
    };
}  // namespace fw