#pragma once

#include "Graphics/Framework.h"
#include "Graphics/RenderManager.h"
#include "Scene/Scene.h"

namespace fw
{
    class Engine
    {
    public:
        Engine(Window& window);
        ~Engine();

        bool Init();
        void Update(f32 dt, f32 total_time);
        // void OnEvent(const Event& e);

        void BeginFrame();
        void EndFrame();

        RenderManager* GetRenderer()
        {
            return &m_RenderManager;
        }
        Window& GetWindow()
        {
            return m_Window;
        }
        Framework& GetFramework()
        {
            return m_Framework;
        }

    private:
        Window& m_Window;
        Framework m_Framework;
        RenderManager m_RenderManager;
        Scene m_Scene;

        f32 m_Fps = 0.0f;
        f32 m_FpsUpdateTimer = 0.0f;
        constexpr static f32 FpsUpdatesPerSecond = 30.0f;
    };
}  // namespace fw