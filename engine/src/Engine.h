#pragma once

#include "Core/Types.h"
#include "Graphics/Framework.h"
#include "Graphics/RenderManager.h"
#include "Scene/Scene.h"

namespace fw
{
    class Engine
    {
    public:
        Engine();
        ~Engine();

        bool Init(std::shared_ptr<Window> window);
        void Update(f32 dt, f32 total_time);
        // void OnEvent(const Event& e);

        void BeginFrame();
        void EndFrame();

        RenderManager* GetRenderer()
        {
            return &m_RenderManager;
        }
        std::shared_ptr<Window> GetWindow()
        {
            return m_Window;
        }
        Framework* GetFramework()
        {
            return &m_Framework;
        }

    private:
        std::shared_ptr<Window> m_Window;
        Framework m_Framework;
        RenderManager m_RenderManager;
        Scene m_Scene;

        f32 m_Fps = 0.0f;
        f32 m_FpsUpdateTimer = 0.0f;
        constexpr static f32 FpsUpdatesPerSecond = 30.0f;
    };
}  // namespace fw