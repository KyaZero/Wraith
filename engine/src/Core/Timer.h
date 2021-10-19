#pragma once

#include <chrono>

#include "Types.h"

namespace Wraith
{
    class Timer
    {
    public:
        Timer()
            : m_DeltaTime(0)
            , m_TotalTime(0)
        {
            m_Time = m_OldTime = m_NewTime = std::chrono::high_resolution_clock::now();
        }
        ~Timer()
        { }

        void Update()
        {
            m_OldTime = m_NewTime;
            m_NewTime = std::chrono::high_resolution_clock::now();

            m_DeltaTime =
                std::chrono::duration_cast<std::chrono::duration<f32, std::ratio<1>>>(m_NewTime - m_OldTime).count();
            m_TotalTime =
                std::chrono::duration_cast<std::chrono::duration<f32, std::ratio<1>>>(m_NewTime - m_Time).count();
        }

        f32 GetDeltaTime()
        {
            return m_DeltaTime;
        }

        f32 GetTotalTime()
        {
            return m_TotalTime;
        }

    private:
        f32 m_DeltaTime;
        f32 m_TotalTime;
        std::chrono::high_resolution_clock::time_point m_Time;
        std::chrono::high_resolution_clock::time_point m_NewTime;
        std::chrono::high_resolution_clock::time_point m_OldTime;
    };
}  // namespace Wraith