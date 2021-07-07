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
        using Job = std::function<void()>;

        PersistentJob(const std::string& id, Job job);

        f32 GetAverageTime() const;
        std::array<f32, TimeLogSize> GetPreviousTimes() const;

        std::string_view GetID() const;

        f32 Execute();

    private:
        std::string m_ID;

        std::array<f32, TimeLogSize> m_PrevTimes;
        f32 m_AverageTime;

        Timer m_Timer;

        Job m_Job;
    };
}  // namespace fw
