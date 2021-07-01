#pragma once

#include <queue>
#include <mutex>

#include "Core/Types.h"

#define VERBOSE_LOG(...) \
    fw::Logger::Get()->Log(::fw::Logger::Level::Verbose, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);
#define INFO_LOG(...) fw::Logger::Get()->Log(::fw::Logger::Level::Info, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);
#define WARNING_LOG(...) \
    fw::Logger::Get()->Log(::fw::Logger::Level::Warning, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);
#define ERROR_LOG(...) \
    fw::Logger::Get()->Log(::fw::Logger::Level::Error, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);

#ifdef NDEBUG
#define ASSERT_LOG(...) ((void)0)
#else
#define ___ASSERT_LOG(...) \
    fw::Logger::Get()->Log(::fw::Logger::Level::Fatal, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__), abort()
#define ASSERT_LOG(expression, ...) \
    (void)((!!(expression)) || (___ASSERT_LOG("Assertion failed: {}", (#expression " " #__VA_ARGS__)), 0))
#endif

namespace fw
{
    class Logger
    {
    public:
        enum class Level : char
        {
            All,
            Verbose,
            Info,
            Warning,
            Error,
            Fatal,
            Count
        };

        Logger();
        ~Logger();

        static void Create(bool multiThreaded = true);
        static void Destroy();
        static Logger* Get();
        static void SetLevel(Level level);
        static void SetPrint(bool shouldPrint);
        static void SetShouldLogToFile(bool shouldLogToFile);

        template <typename... Args>
        void Log(Level level, const char* file, u32 line, const char* function, const char* format, Args&&... args)
        {
            LogInternal(level, file, line, function, std::vformat(format, std::make_format_args(args...)));
        }

    private:
        static Logger* s_Instance;
        void VerifyLogPath();
        void LogToFile(const std::string& msg);
        static void Update(Logger* instance, std::chrono::duration<double, std::milli> interval);

        void LogInternal(Level level, const char* file, u32 line, const char* function, std::string text);

#ifdef _WIN32
        static auto GetLevelString(Level level);
#else
        static std::string GetLevelString(Level level);
#endif

        struct LogEntry
        {
            std::string filename;
            std::string function;
            std::string msg;
            u32 line;
            Level level;
        };
        static void Print(Logger* instance, const LogEntry& entry);

        std::queue<LogEntry> m_Queue;
        std::mutex m_QueueMutex;
        std::unique_ptr<std::thread> m_Thread;
        std::string m_LogPath;

        char m_Level;

        bool m_ShouldPrint;
        bool m_ShouldLogToFile;
        bool m_MultiThreaded;
    };
}  // namespace fw