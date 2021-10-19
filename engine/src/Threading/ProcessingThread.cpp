#include "ProcessingThread.h"

#include "Core/Timer.h"

namespace Wraith
{
    ProcessingThread::ProcessingThread(const std::string& id)
        : Thread(id)
    { }

    ProcessingThread::~ProcessingThread()
    { }

    void ProcessingThread::Queue(std::shared_ptr<ProcessingJob> job)
    {
        std::unique_lock<std::mutex> queue_lock(m_QueueMutex);
        m_QueuedJobs.push_back(job);
        queue_lock.unlock();

        m_Channel.notify_all();
    }

    bool ProcessingThread::HasWork() const
    {
        std::unique_lock<std::mutex> queue_lock(m_QueueMutex);
        return m_IsWorking || !m_QueuedJobs.empty();
    }

    const std::deque<std::shared_ptr<ProcessingJob>>& ProcessingThread::GetQueuedJobs() const
    {
        return m_QueuedJobs;
    }

    void ProcessingThread::Execute()
    {
        SetActive(true);
        while (ShouldRun())
        {
            std::unique_lock<std::mutex> queue_lock(m_QueueMutex);
            if (m_QueuedJobs.empty())
            {
                queue_lock.unlock();

                m_ProjectedTime = 0.0f;

                m_IsWorking = false;

                std::unique_lock<std::mutex> sleep_lock(m_WaitMutex);
                m_Channel.wait(sleep_lock);
            }
            else
            {
                std::shared_ptr<ProcessingJob> work = m_QueuedJobs.front();
                m_QueuedJobs.pop_front();
                queue_lock.unlock();

                m_IsWorking = true;

                Timer timer;
                work->Execute();
                timer.Update();
                m_ProjectedTime = timer.GetTotalTime();
            }
        }
    }
}  // namespace Wraith