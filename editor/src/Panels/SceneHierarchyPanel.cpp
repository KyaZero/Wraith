#include "SceneHierarchyPanel.h"

#include <imgui/imgui.h>

#include "Core/StringID.h"
#include "Scene/Components.h"

namespace Wraith
{
    void SceneHierarchyPanel::OnUIRender()
    {
        if (Begin())
        {
            auto& registry = m_Context.m_Registry;
            registry.each([&](auto e) {
                Entity entity{ e, &registry };
                DrawEntityNode(entity);
            });

            if (ImGui::IsMouseClicked(0) && ImGui::IsWindowHovered())
                SelectEntity({});
        }
        End();
    }

    void SceneHierarchyPanel::DrawEntityNode(Entity entity)
    {
        auto& tag = entity.GetComponent<TagComponent>().tag;

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanFullWidth;

        if (entity == m_SelectedEntity)
            flags |= ImGuiTreeNodeFlags_Selected;

        bool opened = ImGui::TreeNodeEx((void*)(u64)(u32)entity, flags, tag.c_str());
        if (ImGui::IsItemClicked())
        {
            SelectEntity(entity);
        }

        if (opened)
        {
            ImGui::TreePop();
        }
    }
    void SceneHierarchyPanel::SelectEntity(Entity entity)
    {
        m_SelectedEntity = entity;
        m_OnEntitySelected(m_SelectedEntity);
    }
}  // namespace Wraith