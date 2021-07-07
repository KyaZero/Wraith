#include "PersistentJob.h"

namespace fw
{
    PersistentJob::PersistentJob(const std::string& id, std::function<void()> job)
        : m_PrevTimes{ 0 }
        , m_ID(id)
        , m_Job(job)
        , m_AverageTime(0.0f)
    { }
    
    f32 PersistentJob::GetAverageTime() const
    {
        return m_AverageTime;
    }

    std::array<f32, PersistentJob::TimeLogSize> PersistentJob::GetPreviousTimes() const
    {
        return m_PrevTimes;
    }

    const std::string& PersistentJob::GetID() const
    {
        return m_ID;
    }

    f32 PersistentJob::Execute()
    {
        m_Timer.Update();
        m_Job();
        m_Timer.Update();

        f32 time_taken = m_Timer.GetDeltaTime() * 1000.0f;
        f32 total_time = time_taken;
        for (u32 i = 1; i < TimeLogSize; ++i) 
        {
            total_time += m_PrevTimes[i];
            m_PrevTimes[i - 1] = m_PrevTimes[i];
        }
        
        m_PrevTimes[TimeLogSize - 1] = time_taken;
        m_AverageTime = total_time / TimeLogSize;

        return time_taken;
    }
}  // namespace fw
