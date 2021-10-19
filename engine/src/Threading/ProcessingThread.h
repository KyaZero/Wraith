#pragma once

#include "ProcessingJob.h"
#include "Thread.h"

namespace Wraith
{
    class ProcessingThread : public Thread
    {
    public:
        ProcessingThread(const std::string& id);
        virtual ~ProcessingThread();

        void Queue(std::shared_ptr<ProcessingJob> job);

        bool HasWork() const;
        const std::deque<std::shared_ptr<ProcessingJob>>& GetQueuedJobs() const;

    protected:
        void Execute() override final;

    private:
        volatile bool m_IsWorking;
        volatile f32 m_ProjectedTime;

        std::mutex m_WaitMutex;
        mutable std::mutex m_QueueMutex;

        std::deque<std::shared_ptr<ProcessingJob>> m_QueuedJobs;
    };
}  // namespace Wraith