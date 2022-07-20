#include "Scene.h"

#include "Archive.h"
#include "Components.h"
#include "Entity.h"
#include "Graphics/TextureManager.h"
#include "Scene/EditorCamera.h"

namespace Wraith
{
    Scene::Scene() { }

    Scene::~Scene() { }

    bool Scene::Init(RenderManager* renderer)
    {
        m_Renderer = renderer;
        return true;
    }

    Entity Scene::CreateEntity(const std::string& name)
    {
        Entity entity = { m_Registry.create(), &m_Registry };
        entity.AddComponent<TransformComponent>();
        auto& tag = entity.AddComponent<TagComponent>();
        tag.tag = name.empty() ? "Unnamed Entity" : name;
        return entity;
    }

    void Scene::UpdateEditor(f32 dt, EditorCamera* editor_camera)
    {
        m_Renderer->Submit(SetCameraCommand{ editor_camera->GetProjection(), editor_camera->GetViewMatrix() });

        {
            const auto sprites = m_Registry.view<TransformComponent, SpriteComponent>();
            for (const auto entity : sprites)
            {
                const auto [transform, sprite] = sprites.get<TransformComponent, SpriteComponent>(entity);
                m_Renderer->Submit(SpriteCommand{
                    .texture = sprite.texture,
                    .color = sprite.color,
                    .position = transform.position,
                    .origin = sprite.origin,
                    .scale = { transform.scale.x, transform.scale.y },
                    .rotation = transform.rotation.GetEulerAngles().z,
                    .layer = sprite.layer,
                    .screen_space = sprite.screen_space,
                });
            }

            const auto models = m_Registry.view<TransformComponent, ModelComponent>();
            for (const auto entity : models)
            {
                const auto [transform, model] = models.get<TransformComponent, ModelComponent>(entity);
                /*m_Renderer->Submit(ModelCommand{ .model = model.model_instance,
                                                 .transform = Mat4f::CreateTransform(transform.position, transform.rotation, transform.scale) });*/
            }
        }

        {
            const auto view = m_Registry.view<TransformComponent, TextComponent>();
            for (const auto entity : view)
            {
                const auto [transform, text] = view.get<TransformComponent, TextComponent>(entity);
                m_Renderer->Submit(TextCommand{
                    .text = text.text,
                    .font_id = text.font_id,
                    .justification = Justification::FromIndex(text.justification),
                    .alignment = Alignment::FromIndex(text.alignment),
                    .font_size = text.font_size,
                    .direction = text.direction,
                    .color = text.color,
                    .blend_mode = text.blend_mode,
                    .screen_position = { transform.position.x, -transform.position.y },
                });
            }
        }
    }

    void Scene::UpdateRuntime(f32 dt)
    {
        // Script Update
        {
            m_Registry.view<NativeScriptComponent>().each([dt](auto entity, NativeScriptComponent& nsc) {
                if (nsc.instance)
                    nsc.instance->OnUpdate(dt);
            });
        }

        Camera* main_camera = nullptr;
        Mat4f camera_transform;
        const auto cameras = m_Registry.view<TransformComponent, CameraComponent>();
        for (const auto entity : cameras)
        {
            const auto [transform, camera] = cameras.get<TransformComponent, CameraComponent>(entity);

            if (camera.primary)
            {
                main_camera = &camera.camera;
                camera_transform = Mat4f::CreateTransform(transform.position, transform.rotation, transform.scale);
            }
        }

        if (main_camera)
        {
            m_Renderer->Submit(SetCameraCommand{ main_camera->GetProjection(), Mat4f::FastInverse(camera_transform) });

            {
                const auto sprites = m_Registry.view<TransformComponent, SpriteComponent>();
                for (const auto entity : sprites)
                {
                    const auto [transform, sprite] = sprites.get<TransformComponent, SpriteComponent>(entity);
                    m_Renderer->Submit(SpriteCommand{
                        .texture = sprite.texture,
                        .color = sprite.color,
                        .position = transform.position,
                        .origin = sprite.origin,
                        .scale = { transform.scale.x, transform.scale.y },
                        .rotation = transform.rotation.GetEulerAngles().z,
                        .layer = sprite.layer,
                        .screen_space = sprite.screen_space,
                    });
                }

                const auto models = m_Registry.view<TransformComponent, ModelComponent>();
                for (const auto entity : models)
                {
                    const auto [transform, model] = models.get<TransformComponent, ModelComponent>(entity);
                    /*m_Renderer->Submit(ModelCommand{ .model = model.model_instance,
                                                     .transform = Mat4f::CreateTransform(transform.position, transform.rotation, transform.scale) });*/
                }
            }
        }

        {
            const auto view = m_Registry.view<TransformComponent, TextComponent>();
            for (const auto entity : view)
            {
                const auto [transform, text] = view.get<TransformComponent, TextComponent>(entity);
                m_Renderer->Submit(TextCommand{
                    .text = text.text,
                    .font_id = text.font_id,
                    .justification = Justification::FromIndex(text.justification),
                    .alignment = Alignment::FromIndex(text.alignment),
                    .font_size = text.font_size,
                    .direction = text.direction,
                    .color = text.color,
                    .blend_mode = text.blend_mode,
                    .screen_position = { transform.position.x, -transform.position.y },
                });
            }
        }
    }

    void Scene::Play()
    {
        m_Archiver.CreateSnapshot(m_Registry);

        m_Registry.view<NativeScriptComponent>().each([this](auto entity, NativeScriptComponent& nsc) {
            if (!nsc.instance && nsc.InstantiateScript)
            {
                nsc.instance = nsc.InstantiateScript();
                nsc.instance->m_Entity = { entity, &m_Registry };
                nsc.instance->OnCreate();
            }
        });
    }

    void Scene::EndPlay()
    {
        m_Registry.view<NativeScriptComponent>().each([this](auto entity, NativeScriptComponent& nsc) {
            if (nsc.instance)
            {
                nsc.instance->OnDestroy();
                nsc.DestroyScript();
            }
        });

        m_Archiver.RestoreSnapshot(m_Registry);
    }
}  // namespace Wraith