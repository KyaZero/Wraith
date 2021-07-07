#include "Thread.h"

#include <thread>

#ifdef WIN32
// Apparently this is the only way to name threads. lol
static void SetThreadName(u32 dwThreadID, const char* threadName)
{
    struct
    {
        DWORD dwType = 0x1000;  // Must be 0x1000.
        LPCSTR szName;          // Pointer to name (in user addr space).
        DWORD dwThreadID;       // Thread ID (-1=caller thread).
        DWORD dwFlags = 0;      // Reserved for future use, must be zero.
    } info;

    info.szName = threadName;
    info.dwThreadID = dwThreadID;

    __try
    {
        RaiseException(0x406D1388, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    { }
}
#endif

namespace fw
{
    Thread::Thread(const std::string& id)
        : m_ShouldRun(true)
        , m_IsRunning(false)
        , m_Channel()
        , m_Mutex()
    {
        m_Job = [this] { Execute(); };
        m_Thread = std::make_unique<std::thread>(&Thread::ExecuteInternal, this);

        SetID(id);
    }
    Thread::~Thread()
    {
        m_Thread->join();
        VERBOSE_LOG("Thread {} is dead", m_ID);
    }

    const std::string& Thread::GetID() const
    {
        return m_ID;
    }

    bool Thread::IsRunning() const
    {
        return m_IsRunning;
    }

    bool Thread::ShouldRun() const
    {
        return m_ShouldRun;
    }

    void Thread::Kill()
    {
        VERBOSE_LOG("Signalling thread {} to die", m_ID);
        m_ShouldRun = false;
        m_Channel.notify_all();
    }

    void Thread::SetActive(bool active)
    {
        m_IsRunning = active;
    }

    void Thread::SetID(const std::string& id)
    {
        m_ID = id;

#ifdef WIN32
        SetThreadName(::GetThreadId((HANDLE)m_Thread->native_handle()), id.c_str());
#endif
    }

    void Thread::ExecuteInternal()
    {
        m_ShouldRun = true;
        VERBOSE_LOG("Starting Thread {}...", m_ID);

        m_Job();
    }
}  // namespace fw