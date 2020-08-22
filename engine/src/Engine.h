#pragma once
#include "Core/Types.h"
#include "Window/Event.h"
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

        bool Init(Window* window);
        void Update(f32 dt, f32 total_time);
        void OnEvent(const Event& e);

    private:
        Window* m_Window;
        Framework m_Framework;
        RenderManager m_RenderManager;
        Scene m_Scene;

        //for averaging out fps
        std::deque<f32> m_LastTimes;
        constexpr static i32 MaxNumTimesSaved = 20;
    };
}