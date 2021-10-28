#pragma once
#include "PersistentJob.h"
#include "Thread.h"

namespace Wraith
{
    class WorkerThread : public Thread
    {
    public:
        WorkerThread(const std::string& id);
        virtual ~WorkerThread();

        void Clear();
        void Queue(std::shared_ptr<PersistentJob> job);
        void Run();

        const f32 GetProjectedTime() const;

    protected:
        void Execute() override final;

    private:
        f32 m_ProjectedTime;
        std::vector<std::shared_ptr<PersistentJob>> m_QueuedJobs;
    };
}  // namespace Wraith