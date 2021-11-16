#include "ThreadScheduler.h"

#include "imgui/imgui.h"

namespace Wraith
{
    ThreadScheduler::ThreadScheduler()
        : m_FrameIndex(0)
    {
        // Leaving atleast one thread for the main loop
        i32 num_logical_cores = std::thread::hardware_concurrency() - 1;

        // Half the threads for persistent jobs, and half for processing jobs
        i32 num_threads = std::max(num_logical_cores / 2, 1);
        INFO_LOG("Starting {} worker threads ({} current and {} buffered)", num_threads * 2, num_threads, num_threads);

        for (size_t frame = 0; frame < FRAME_COUNT; frame++)
        {
            m_WorkerThreads[frame].resize(num_threads);

            for (i32 i = 0; i < num_threads; ++i)
            {
                m_WorkerThreads[frame][i] = std::make_unique<WorkerThread>(std::format("Worker Thread #{}", i));
            }
        }

        m_ProcessingThreads.resize(num_threads);

        for (i32 i = 0; i < num_threads; ++i)
        {
            m_ProcessingThreads[i] = std::make_unique<ProcessingThread>(std::format("Processing Thread #{}", i));
        }

        m_NumThreads = num_threads;

        AddPersistentJob(std::make_shared<PersistentJob>("Schedule Frame", [this]() { ScheduleFrame(); }));

        // Must schedule first frame manually, so that the Schedule Job actually gets scheduled
        ScheduleFrame();
    }

    ThreadScheduler::~ThreadScheduler()
    {
        for (size_t frame = 0; frame < FRAME_COUNT; frame++)
        {
            for (i32 i = 0; i < m_NumThreads; ++i)
            {
                m_WorkerThreads[frame][i]->Kill();
            }
            m_WorkerThreads[frame].clear();
        }

        for (i32 i = 0; i < m_NumThreads; ++i)
        {
            m_ProcessingThreads[i]->Kill();
        }
        m_ProcessingThreads.clear();

        INFO_LOG("Thread pool destroyed!");
    }

    void ThreadScheduler::Start()
    {
        std::swap(m_WorkerThreads[CURRENT_FRAME], m_WorkerThreads[NEXT_FRAME]);

        for (i32 i = 0; i < m_NumThreads; ++i)
        {
            m_WorkerThreads[CURRENT_FRAME][i]->Run();
        }
    }

    void ThreadScheduler::Sync()
    {
        bool done = false;
        while (!done)
        {
            done = true;
            for (i32 i = 0; i < m_NumThreads; ++i)
            {
                if (m_WorkerThreads[CURRENT_FRAME][i]->IsRunning())
                {
                    done = false;
                }
            }
        }

        for (i32 i = 0; i < m_NumThreads; ++i)
        {
            m_WorkerThreads[CURRENT_FRAME][i]->Clear();
        }
    }

    void ThreadScheduler::AddProcessingJob(std::shared_ptr<ProcessingJob> job) { m_ProcessingJobs.push_back(job); }

    void ThreadScheduler::AddPersistentJob(std::shared_ptr<PersistentJob> job) { m_PersistentJobs.push_back(job); }

    void ThreadScheduler::ScheduleFrame()
    {
        m_FrameIndex++;

        std::sort(m_PersistentJobs.begin(),
                  m_PersistentJobs.end(),
                  [](const std::shared_ptr<PersistentJob> a, const std::shared_ptr<PersistentJob> b) {
                      return a->GetAverageTime() < b->GetAverageTime();
                  });

        i32 thread_index = 0;
        for (auto& job : m_PersistentJobs)
        {
            auto index = thread_index++ % m_NumThreads;
            m_WorkerThreads[NEXT_FRAME][index]->Queue(job);
        }

        for (auto& job : m_ProcessingJobs)
        {
            auto index = thread_index++ % m_NumThreads;
            m_ProcessingThreads[index]->Queue(job);
        }
        m_ProcessingJobs.clear();
    }

#ifdef WITH_EDITOR
    void ThreadScheduler::RenderDebugInfo()
    {
        if (ImGui::Button("Spawn Test Processing Work"))
        {
            for (size_t i = 0; i < 200; i++)
            {
                AddProcessingJob(std::make_shared<ProcessingJob>(
                    std::format("Test Load #{}", i),
                    [](auto) { std::this_thread::sleep_for(std::chrono::milliseconds(RandomRange(100, 500))); },
                    [](ProcessingJob& job) { INFO_LOG("'{}' took {}s", job.GetID(), job.GetExecutionTime()); }));
            }
        }

        static bool show_all = false;
        ImGui::Checkbox("Show All Threads", &show_all);

        if (ImGui::TreeNodeEx("Persistent Jobs: ", ImGuiTreeNodeFlags_DefaultOpen))
        {
            f32 total_time = 0.0f;
            for (auto& job : m_PersistentJobs)
            {
                ImGui::Text(std::format("\t{}: {:.3f}ms", job->GetID(), job->GetAverageTime()).c_str());
                total_time += job->GetAverageTime();
            }
            ImGui::TreePop();
        }

        if (ImGui::TreeNodeEx("Processing Jobs: ", ImGuiTreeNodeFlags_DefaultOpen))
        {
            for (auto& thread : m_ProcessingThreads)
            {
                if (!thread->HasWork())
                    continue;

                for (auto& job : thread->GetQueuedJobs())
                {
                    ImGui::Text(std::format("\t{}", job->GetID()).c_str());
                }
            }
            ImGui::TreePop();
        }

        if (ImGui::TreeNodeEx("Threads: ", ImGuiTreeNodeFlags_DefaultOpen))
        {
            for (auto& thread : m_WorkerThreads[NEXT_FRAME])
            {
                if (thread->GetProjectedTime() <= 0.0f && !show_all)
                    continue;

                ImGui::Text(std::format("\t{}: {:.3f}ms", thread->GetID(), thread->GetProjectedTime()).c_str());
            }

            for (auto& thread : m_ProcessingThreads)
            {
                if (!thread->HasWork() && !show_all)
                    continue;

                ImGui::Text(std::format("\t{}: {}", thread->GetID(), thread->HasWork() ? "Working" : "Idle").c_str());
            }
            ImGui::TreePop();
        }
    }
#endif
}  // namespace Wraith