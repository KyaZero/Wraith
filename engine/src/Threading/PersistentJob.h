#pragma once
#include <functional>

#include "Core/Timer.h"
#include "Core/Types.h"

namespace fw
{
    // A job which happens every frame, e.g Update/Render/etc...
    class PersistentJob
    {
    public:
        constexpr static u32 TimeLogSize = 60;

        PersistentJob(const std::string& id, std::function<void()> job);

        f32 GetAverageTime() const;
        std::array<f32, TimeLogSize> GetPreviousTimes() const;

        const std::string& GetID() const;

        f32 Execute();

    private:
        std::string m_ID;

        std::array<f32, TimeLogSize> m_PrevTimes;
        f32 m_AverageTime;

        Timer m_Timer;

        std::function<void()> m_Job;
    };
}  // namespace fw