#include "Editor.h"

#include <imgui/imgui.h>

#include "Scene/Components.h"
#include "Scene/Entity.h"
#include "Scene/ScriptableEntity.h"

namespace Wraith
{
    Editor::Editor()
        : Application("Editor")
        , m_CameraController(m_Engine->GetWindow().GetSize().x, m_Engine->GetWindow().GetSize().y, true)
    {
        m_ActiveScene = std::make_unique<Scene>();
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
                sprite.texture = StringID("assets/engine/textures/default.png");
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
            CameraController(f32 speed)
                : m_Speed(speed)
            { }

        protected:
            void OnCreate()
            { }

            void OnDestroy()
            { }

            void OnUpdate(f32 dt)
            {
                auto& transform = GetComponent<TransformComponent>();
                if (GetInput().IsDown(Key::A))
                    transform.position.x -= m_Speed * dt;
                if (GetInput().IsDown(Key::D))
                    transform.position.x += m_Speed * dt;
                if (GetInput().IsDown(Key::W))
                    transform.position.y -= m_Speed * dt;
                if (GetInput().IsDown(Key::S))
                    transform.position.y += m_Speed * dt;
            }

        private:
            f32 m_Speed;
        };

        m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>(10.f);

        {
            m_BarPanel = std::make_unique<BarPanel>([this] { OnScenePlay(); }, [this] { OnSceneEndPlay(); });
            m_PanelManager.AddPanel(m_BarPanel.get());

            m_ViewportPanel = std::make_unique<ViewportPanel>(*m_Engine->GetRenderer(), m_CameraController);
            m_PanelManager.AddPanel(m_ViewportPanel.get());

            m_SceneHierarchyPanel = std::make_unique<SceneHierarchyPanel>(
                *m_ActiveScene, [this](auto entity) { m_PropertiesPanel->SetSelectedEntity(entity); });
            m_PanelManager.AddPanel(m_SceneHierarchyPanel.get());

            m_PropertiesPanel = std::make_unique<PropertiesPanel>();
            m_PanelManager.AddPanel(m_PropertiesPanel.get());
        }

        {
            auto textEntity = m_ActiveScene->CreateEntity("Text");
            textEntity.AddComponent<TextComponent>();
        }
    }

    void Editor::OnUpdate(f32 dt)
    {
        if (m_IsScenePlaying)
            m_ActiveScene->UpdateRuntime(dt);
        else
        {
            if (m_ViewportPanel->IsFocused())
                m_CameraController.Update(dt);
            m_ActiveScene->UpdateEditor(dt, m_CameraController.GetCamera());
        }
    }

    void Editor::OnUIRender()
    {
        m_PanelManager.OnUIRender();
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
}  // namespace Wraith