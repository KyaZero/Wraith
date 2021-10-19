#pragma once

#include <entt/entt.hpp>

#include "Graphics/OrthographicCameraController.h"
#include "Graphics/RenderManager.h"
#include "Scene/Archive.h"
#include "Scene/Components.h"
#include "Window/Window.h"

namespace Wraith
{
    class Entity;
    class Scene
    {
        friend class Entity;
        friend class SceneHierarchyPanel;

    public:
        Scene();
        ~Scene();

        bool Init(RenderManager* renderer);

        Entity CreateEntity(const std::string& name = std::string());

        void UpdateEditor(f32 dt, OrthographicCamera* editor_camera);
        void UpdateRuntime(f32 dt);

        void Play();
        void EndPlay();

    private:
        RenderManager* m_Renderer;
        bool m_IsPlaying;
        entt::registry m_Registry;

        Archiver<TagComponent, TransformComponent, SpriteComponent, CameraComponent, NativeScriptComponent> m_Archiver;
    };
}  // namespace Wraith