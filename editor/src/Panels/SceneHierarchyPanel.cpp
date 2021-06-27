#include "SceneHierarchyPanel.h"

#include <imgui.h>

#include "Scene/Components.h"

namespace fw
{
	SceneHierarchyPanel::SceneHierarchyPanel(std::shared_ptr<Scene> context)
	    : m_SelectionContext()
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(std::shared_ptr<Scene> context)
	{
		m_Context = context;
	}

	void SceneHierarchyPanel::OnUIRender()
	{
		ImGui::Begin("Scene Hierarchy");

		auto& registry = m_Context->m_Registry;
		registry.each([&](auto e) {
			Entity entity{ e, m_Context.get() };
			DrawEntityNode(entity);
		});

		if (ImGui::IsMouseClicked(0) && ImGui::IsWindowHovered())
			m_SelectionContext = {};

		ImGui::End();

		// Move this stuff to PropertiesPanel
		ImGui::Begin("Properties");
		if (m_SelectionContext)
		{
			DrawComponents(m_SelectionContext);
		}
		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().tag;

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;

		if (entity == m_SelectionContext)
			flags |= ImGuiTreeNodeFlags_Selected;

		bool opened = ImGui::TreeNodeEx((void*)(u64)(u32)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}

		if (opened)
		{
			ImGui::TreePop();
		}
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		if (entity.HasComponent<TransformComponent>())
		{
			if (ImGui::TreeNodeEx(
			        (void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				auto& transform = entity.GetComponent<TransformComponent>();

				ImGui::DragFloat3("Position", &transform.position.x, 0.01f);
				ImGui::DragFloat3("Scale", &transform.scale.x, 0.001f, 0.001f, 10.0f);
				Vec3f rotation_in_degrees = { Degrees(transform.rotation.x),
					                          Degrees(transform.rotation.y),
					                          Degrees(transform.rotation.z) };
				ImGui::DragFloat3("Rotation", &rotation_in_degrees.x, 0.1f);
				transform.rotation = { Radians(rotation_in_degrees.x),
					                   Radians(rotation_in_degrees.y),
					                   Radians(rotation_in_degrees.z) };
				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<CameraComponent>())
		{
			if (ImGui::TreeNodeEx(
			        (void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera Component"))
			{
				// auto& camera_component = entity.GetComponent<CameraComponent>();

				ImGui::TreePop();
			}
		}
	}
}  // namespace fw