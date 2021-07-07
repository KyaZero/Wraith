#pragma once
#include "Core/Types.h"
#include "Core/Timer.h"
#include <functional>

namespace fw
{
    class PersistentJob
    {
    public:
        constexpr static u32 TimeLogSize = 60;

        PersistentJob(
            const std::string& id, std::function<void()> job, std::function<void()> on_flip = []() {});

        f32 GetAverageTime() const;
        std::array<f32, TimeLogSize> GetPreviousTimes() const;

        const std::string& GetID() const;

        f32 Execute();

        void Flip();

    private:
        std::string m_ID;

        std::array<f32, TimeLogSize> m_PrevTimes;
        volatile f32 m_AverageTime;

        Timer m_Timer;

        std::function<void()> m_Job;
        std::function<void()> m_OnFlip;
    };
}  // namespace fw