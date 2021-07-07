#pragma once

namespace fw
{
    class ProcessingJob
    {
    public:
        ProcessingJob(
            const std::string& id,
            std::function<void(ProcessingJob&)> job,
            std::function<void(ProcessingJob&)> callback = [](ProcessingJob& job) { });
        ~ProcessingJob();

        bool IsDone() const;
        f32 GetExecutionTime() const;

        const std::string& GetID() const;

        void Execute();

        //For passing whatever userdata you might need to the job
        void SetUserData(void* data);
        void* GetUserData() const;

    private:
        std::string m_ID;

        std::function<void(ProcessingJob&)> m_Job;
        std::function<void(ProcessingJob&)> m_OnFinish;

        volatile bool m_Done;
        f32 m_ExecutionTime;

        void* m_UserData;
    };
}  // namespace fw