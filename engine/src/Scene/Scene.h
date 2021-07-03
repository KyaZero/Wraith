#pragma once

#include <dubu_serialize/dubu_serialize.h>
#include <entt/entt.hpp>

#include "Graphics/OrthographicCameraController.h"
#include "Graphics/RenderManager.h"
#include "Window/Window.h"

namespace fw
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

        dubu::serialize::MemoryBuffer m_Buffer;
    };
}  // namespace fw