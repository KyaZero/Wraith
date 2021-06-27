#pragma once
#include "Core/Types.h"
#include "Graphics/Framework.h"
#include "Graphics/RenderManager.h"
#include "Scene/Scene.h"
#include <queue>

namespace fw
{
    class Engine
    {
    public:
        Engine();
        ~Engine();

        bool Init(std::shared_ptr<Window> window);
        void Update(f32 dt, f32 total_time);
        //void OnEvent(const Event& e);

        void BeginFrame();
        void EndFrame();

        RenderManager* GetRenderer() { return &m_RenderManager; }
        std::shared_ptr<Window> GetWindow() { return m_Window; }
        Framework* GetFramework() { return &m_Framework; }

    private:
        std::shared_ptr<Window> m_Window;
        Framework m_Framework;
        RenderManager m_RenderManager;
        Scene m_Scene;

        //for averaging out fps
        std::deque<f32> m_LastTimes;
        constexpr static i32 MaxNumTimesSaved = 20;
    };
}