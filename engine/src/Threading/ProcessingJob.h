#pragma once

namespace fw
{
    class ProcessingJob
    {
    public:
        using Job = std::function<void(ProcessingJob&)>;
        ProcessingJob(
            const std::string& id,
            Job job,
            Job callback = [](ProcessingJob&) { });
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

        Job m_Job;
        Job m_OnFinish;

        volatile bool m_Done;
        f32 m_ExecutionTime;

        void* m_UserData;
    };
}  // namespace fw
