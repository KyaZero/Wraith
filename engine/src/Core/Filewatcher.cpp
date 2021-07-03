#include "Filewatcher.h"

namespace fw
{
    Filewatcher::Filewatcher()
    {
        m_HasChanges = false;
        m_Thread = std::jthread([=]() { WatchFiles(); });
    }

    void Filewatcher::Watch(const std::string& path, WatchCallback callback)
    {
        std::filesystem::file_time_type last_write_time;
        if (std::filesystem::exists(path))
            last_write_time = std::filesystem::last_write_time(path);

        m_Entries[path] = FileEntry{ .callback = callback, .last_write_time = last_write_time };
    }

    void Filewatcher::FlushChanges()
    {
        if (!m_HasChanges)
            return;

        for (auto& path : m_Changes)
        {
            INFO_LOG("Reloading path: '{}'", path.c_str());
            m_Entries[path].callback();
        }

        m_Changes.clear();
        m_HasChanges = false;
    }

    void Filewatcher::WatchFiles()
    {
        while (!m_Thread.get_stop_token().stop_requested())
        {
            if (!m_HasChanges)
            {
                for (auto& entry : m_Entries)
                {
                    if (!std::filesystem::exists(entry.first))
                        continue;

                    auto current_write_time = std::filesystem::last_write_time(entry.first);
                    if ((current_write_time - entry.second.last_write_time).count() == 0)
                        continue;
                    entry.second.last_write_time = current_write_time;
                    m_Changes.push_back(entry.first);
                }
            }

            if (m_Changes.size() > 0)
                m_HasChanges = true;

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
}  // namespace fw