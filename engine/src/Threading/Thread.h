#pragma once
#include <thread>

namespace Wraith
{
    class Thread
    {
    public:
        Thread(const std::string& id);
        virtual ~Thread();

        std::string_view GetID() const;

        bool IsRunning() const;
        bool ShouldRun() const;
        virtual void Kill();

    protected:
        void SetActive(bool active);
        virtual void Execute() = 0;

        std::mutex m_Mutex;
        std::condition_variable m_Channel;

    private:
        void SetID(const std::string& id);
        void ExecuteInternal();

        std::function<void()> m_Job;

        std::unique_ptr<std::thread> m_Thread;
        std::string m_ID;

        volatile bool m_IsRunning;
        volatile bool m_ShouldRun;
    };
}  // namespace fw
