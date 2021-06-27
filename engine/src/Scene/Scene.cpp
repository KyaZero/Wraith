#include "Scene.h"

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
        Entity entity = { m_Registry.create(), this };
        entity.AddComponent<TransformComponent>();
        auto& tag = entity.AddComponent<TagComponent>();
        tag.tag = name.empty() ? "Unnamed Entity" : name;
        return entity;
    }

    void Scene::UpdateEditor(f32 dt, OrthographicCamera* editor_camera)
    {
        m_Renderer->Submit(SetCameraCommand{ editor_camera, editor_camera->GetView() });

        auto view = m_Registry.view<TransformComponent, SpriteComponent>();
        for (auto& entity : view)
        {
            auto [transform, sprite] = view.get<TransformComponent, SpriteComponent>(entity);
            SpriteCommand sprite_cmd{
                sprite.texture,
                sprite.color,
                { transform.position.x, transform.position.y },
                sprite.origin,
                { transform.scale.x, transform.scale.y },
                transform.rotation.z,
                sprite.layer,
                sprite.world_space,
            };

            m_Renderer->Submit(sprite_cmd);
        }
    }

    void Scene::UpdateRuntime(f32 dt)
    {
        // Script Update
        {
            m_Registry.view<NativeScriptComponent>().each([=](auto entity, NativeScriptComponent& nsc) {
                if (nsc.instance)
                    nsc.instance->OnUpdate(dt);
            });
        }

        Camera* main_camera = nullptr;
        Mat4f camera_transform;
        auto cameras = m_Registry.view<TransformComponent, CameraComponent>();
        for (auto& entity : cameras)
        {
            auto [transform, camera] = cameras.get<TransformComponent, CameraComponent>(entity);

            if (camera.primary)
            {
                main_camera = &camera.camera;
                camera_transform = Mat4f::CreateTransform(transform.position, transform.rotation, transform.scale);
            }
        }

        if (main_camera)
        {
            m_Renderer->Submit(SetCameraCommand{ main_camera, Mat4f::FastInverse(camera_transform) });

            auto view = m_Registry.view<TransformComponent, SpriteComponent>();
            for (auto& entity : view)
            {
                auto [transform, sprite] = view.get<TransformComponent, SpriteComponent>(entity);
                SpriteCommand sprite_cmd{
                    sprite.texture,
                    sprite.color,
                    { transform.position.x, transform.position.y },
                    sprite.origin,
                    { transform.scale.x, transform.scale.y },
                    transform.rotation.z,
                    sprite.layer,
                    sprite.world_space,
                };

                m_Renderer->Submit(sprite_cmd);
            }
        }
    }

    void Scene::Play()
    {
        m_Registry.view<NativeScriptComponent>().each([=](auto entity, NativeScriptComponent& nsc) {
            if (!nsc.instance)
            {
                nsc.instance = nsc.InstantiateScript();
                nsc.instance->m_Entity = { entity, this };
                nsc.instance->OnCreate();
            }
        });
    }

    void Scene::EndPlay()
    {
        m_Registry.view<NativeScriptComponent>().each([=](auto entity, NativeScriptComponent& nsc) {
            if (nsc.instance)
            {
                nsc.instance->OnDestroy();
                nsc.DestroyScript(&nsc);
            }
        });
    }
}  // namespace fw