#include "WorkerThread.h"

#include "Engine.h"

namespace Wraith
{
    WorkerThread::WorkerThread(const std::string& id)
        : Thread(id)
    {
        Clear();
    }

    WorkerThread::~WorkerThread() { }

    void WorkerThread::Clear()
    {
        m_ProjectedTime = 0;
        m_QueuedJobs.clear();
    }

    void WorkerThread::Queue(std::shared_ptr<PersistentJob> job)
    {
        if (ShouldRun())
        {
            m_QueuedJobs.push_back(job);
            m_ProjectedTime += job->GetAverageTime();
        }
    }

    void WorkerThread::Run()
    {
        SetActive(true);
        m_Channel.notify_all();
    }

    const f32 WorkerThread::GetProjectedTime() const { return m_ProjectedTime; }

    void WorkerThread::Execute()
    {
        while (ShouldRun())
        {
            std::unique_lock<std::mutex> wait_lock(m_Mutex);
            while (!IsRunning() && ShouldRun())
            {
                m_Channel.wait(wait_lock);
            }

            wait_lock.unlock();

            if (!ShouldRun())
                return;

            f32 accumulated_time = 0.0f;
            for (auto& job : m_QueuedJobs)
                accumulated_time += job->Execute();

            SetActive(false);
        }
    }
}  // namespace Wraith