#include "Editor.h"

#include "Scene/Components.h"
#include "Scene/Entity.h"
#include "Scene/ScriptableEntity.h"
#include "imgui.h"

namespace fw
{
	Editor::Editor()
	    : Application("Editor")
	    , m_CameraController(m_Engine->GetWindow()->GetSize().x, m_Engine->GetWindow()->GetSize().y, true)
	    , m_ActiveScene(nullptr)
	{
		m_ActiveScene = std::make_shared<Scene>();
		m_ActiveScene->Init(m_Engine->GetRenderer());

		const i32 num = 10;
		const i32 half_num = num / 2;

		for (i32 y = -half_num; y < half_num; ++y)
		{
			for (i32 x = -half_num; x < half_num; ++x)
			{
				auto e =
				    m_ActiveScene->CreateEntity("Sprite_" + std::to_string((x + half_num) + ((y + half_num) * num)));
				auto& sprite = e.AddComponent<SpriteComponent>();
				sprite.texture = TextureID("assets/engine/textures/default.png");
				sprite.origin = { 0.5f, 0.5f };
				sprite.color =
				    Vec4f{ x / (f32)half_num, y / (f32)half_num, (x / (f32)half_num + y / (f32)half_num) / 2.0f, 1 } +
				    Vec4f{ 1.1f, 1.1f, 1.1f, 0.0f };
				sprite.layer = y;
				sprite.world_space = true;

				auto& transform = e.GetComponent<TransformComponent>();
				transform.position = { (f32)x, (f32)y, 0.0f };
				transform.scale = Vec3f{ 1.0f / 512.0f, 1.0f / 512.0f, 1.0f };
			}
		}

		m_CameraEntity = m_ActiveScene->CreateEntity("Runtime Camera");
		m_CameraEntity.AddComponent<CameraComponent>(
		    Camera(Mat4f::CreatePerspectiveProjection(90.0f, -16.0f / 9.0f, 0.01f, 100.0f)));
		m_CameraEntity.GetComponent<TransformComponent>().position = { 0.0f, 0.0f, -10.0f };

		class CameraController : public ScriptableEntity
		{
		public:
			void OnCreate()
			{ }

			void OnDestroy()
			{ }

			void OnUpdate(f32 dt)
			{
				auto& transform = GetComponent<TransformComponent>();
				float speed = 4.0f;

				if (GetInput().IsDown(Key::A))
					transform.position.x -= speed * dt;
				if (GetInput().IsDown(Key::D))
					transform.position.x += speed * dt;
				if (GetInput().IsDown(Key::W))
					transform.position.y -= speed * dt;
				if (GetInput().IsDown(Key::S))
					transform.position.y += speed * dt;
			}
		};

		m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void Editor::OnUpdate(f32 dt)
	{
		if (m_IsScenePlaying)
			m_ActiveScene->UpdateRuntime(dt);
		else
		{
			if (m_ViewportFocused)
				m_CameraController.Update(dt);
			m_ActiveScene->UpdateEditor(dt, m_CameraController.GetCamera());
		}
	}

	void Editor::OnUIRender()
	{
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

		if (m_ViewportSize.x != viewportPanelSize.x || m_ViewportSize.y != viewportPanelSize.y)
		{
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

			m_Engine->GetRenderer()->Resize(m_ViewportSize.x, m_ViewportSize.y);
			m_CameraController.Resize(m_ViewportSize.x, m_ViewportSize.y);
			// m_ActiveScene->Resize(m_ViewportSize.x, m_ViewportSize.y);
		}

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Input::BlockUIEvents(!m_ViewportFocused && !m_ViewportHovered);

		std::shared_ptr<Texture> texture = m_Engine->GetRenderer()->GetRenderTexture();
		// Draw first
		ImGui::Image(texture->GetShaderResourceView(),
		             ImVec2{ m_ViewportSize.x, m_ViewportSize.y },
		             ImVec2{ 0, 0 },
		             ImVec2{ 1, 1 });

		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::Begin("Bar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoTitleBar);
		if (ImGui::Button(m_IsPlay ? "Play" : "End Play"))
		{
			if (m_IsPlay)
				OnScenePlay();
			else
				OnSceneEndPlay();

			m_IsPlay = !m_IsPlay;
		}
		ImGui::End();

		m_SceneHierarchyPanel.OnUIRender();
	}

	void Editor::OnScenePlay()
	{
		m_ActiveScene->Play();
		m_IsScenePlaying = true;
	}

	void Editor::OnSceneEndPlay()
	{
		m_ActiveScene->EndPlay();
		m_IsScenePlaying = false;
	}
}  // namespace fw