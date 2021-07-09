#pragma once
#include "Core/Constants.h"
#include "Core/Singleton.h"
#include "ProcessingThread.h"
#include "WorkerThread.h"

namespace fw
{
    class ThreadScheduler : public Singleton<ThreadScheduler>
    {
    public:
        ThreadScheduler();
        ~ThreadScheduler();

        void Start();
        void Sync();

        void RenderDebugInfo();

        void AddProcessingJob(std::shared_ptr<ProcessingJob> job);
        void AddPersistentJob(std::shared_ptr<PersistentJob> job);

    private:
        void ScheduleFrame();

        i32 m_NumThreads;
        std::vector<std::unique_ptr<WorkerThread>> m_WorkerThreads[FRAME_COUNT];
        std::vector<std::unique_ptr<ProcessingThread>> m_ProcessingThreads;

        u64 m_FrameIndex;

        std::vector<std::shared_ptr<PersistentJob>> m_PersistentJobs;
        std::vector<std::shared_ptr<ProcessingJob>> m_ProcessingJobs;
    };
}  // namespace fw