#pragma once

#include "Logger.h"
#include "Singleton.h"

namespace fw
{
    using WatchCallback = std::function<void()>;

    class Filewatcher : public Singleton<Filewatcher>
    {
    public:
        struct FileEntry
        {
            WatchCallback callback;
            std::filesystem::file_time_type last_write_time;
        };
        Filewatcher();
        ~Filewatcher();

        void Watch(const std::string& path, WatchCallback callback);
        void FlushChanges();

    private:
        void WatchFiles();

        std::unordered_map<std::string, FileEntry> m_Entries;
        std::vector<std::string> m_Changes;
        std::jthread m_Thread;

        volatile bool m_IsRunning;
        volatile bool m_HasChanges;
    };
}  // namespace fw