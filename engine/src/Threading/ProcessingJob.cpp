#include "ProcessingJob.h"

#include "Core/Timer.h"

namespace fw
{
    ProcessingJob::ProcessingJob(const std::string& id,
                                 ProcessingJob::Job job,
                                 ProcessingJob::Job callback)
        : m_ID(id)
        , m_Job(job)
        , m_OnFinish(callback)
        , m_Done(false)
        , m_ExecutionTime(0.0f)
        , m_UserData(nullptr)
    { }

    ProcessingJob::~ProcessingJob()
    { }

    bool ProcessingJob::IsDone() const
    {
        return m_Done;
    }

    f32 ProcessingJob::GetExecutionTime() const
    {
        return m_ExecutionTime;
    }

    std::string_view ProcessingJob::GetID() const
    {
        return m_ID;
    }

    void ProcessingJob::Execute()
    {
        Timer timer;

        m_Job(*this);

        timer.Update();
        m_ExecutionTime = timer.GetTotalTime();
        m_Done = true;

        m_OnFinish(*this);
    }

    void ProcessingJob::SetUserData(void* data)
    {
        m_UserData = data;
    }

    void* ProcessingJob::GetUserData() const
    {
        return m_UserData;
    }
}  // namespace fw
