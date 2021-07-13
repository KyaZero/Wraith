#include "Scene.h"

#include "Archive.h"
#include "Components.h"
#include "Entity.h"
#include "Graphics/TextureManager.h"

namespace fw
{
    Scene::Scene()
    { }

    Scene::~Scene()
    { }

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

    void Scene::UpdateEditor(f32 dt, OrthographicCamera* editor_camera)
    {
        m_Renderer->Submit(SetCameraCommand{ editor_camera, editor_camera->GetView() });

        {
            const auto view = m_Registry.view<TransformComponent, SpriteComponent>();
            for (const auto entity : view)
            {
                const auto [transform, sprite] = view.get<TransformComponent, SpriteComponent>(entity);
                SpriteCommand sprite_cmd{
                    .texture = sprite.texture,
                    .color = sprite.color,
                    .position = { transform.position.x, transform.position.y },
                    .origin = sprite.origin,
                    .scale = { transform.scale.x, transform.scale.y },
                    .rotation = transform.rotation.z,
                    .layer = sprite.layer,
                    .world_space = sprite.world_space,
                };

                m_Renderer->Submit(sprite_cmd);
            }
        }

        {
            const auto view = m_Registry.view<TextComponent>();
            for (const auto entity : view)
            {
                const auto& text = view.get<TextComponent>(entity);
                m_Renderer->Submit(TextCommand{
                    .text = text.text,
                    .font_id = text.font_id,
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
            m_Renderer->Submit(SetCameraCommand{ main_camera, Mat4f::FastInverse(camera_transform) });

            {
                const auto view = m_Registry.view<TransformComponent, SpriteComponent>();
                for (const auto entity : view)
                {
                    const auto [transform, sprite] = view.get<TransformComponent, SpriteComponent>(entity);
                    SpriteCommand sprite_cmd{
                        .texture = sprite.texture,
                        .color = sprite.color,
                        .position = { transform.position.x, transform.position.y },
                        .origin = sprite.origin,
                        .scale = { transform.scale.x, transform.scale.y },
                        .rotation = transform.rotation.z,
                        .layer = sprite.layer,
                        .world_space = sprite.world_space,
                    };

                    m_Renderer->Submit(sprite_cmd);
                }
            }
        }

        {
            const auto view = m_Registry.view<TextComponent>();
            for (const auto entity : view)
            {
                const auto& text = view.get<TextComponent>(entity);
                m_Renderer->Submit(TextCommand{
                    .text = text.text,
                    .font_id = text.font_id,
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
}  // namespace fw