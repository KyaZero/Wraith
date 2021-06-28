#pragma once

#include <queue>

#include "Core/Types.h"
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

        // for averaging out fps
        std::deque<f32> m_LastTimes;
        constexpr static i32 MaxNumTimesSaved = 20;
    };
}  // namespace fw