#include "Filewatcher.h"

namespace fw
{
    Filewatcher* Filewatcher::m_Instance = nullptr;

    Filewatcher* Filewatcher::Get()
    {
        return m_Instance;
    }

    void Filewatcher::Create()
    {
        if (m_Instance)
            ERROR_LOG("Trying to create a filewatcher when it's already been created!");
        m_Instance = new Filewatcher;
    }

    void Filewatcher::Destroy()
    {
        if (m_Instance)
            delete m_Instance;
    }

    Filewatcher::Filewatcher()
    {
        m_IsRunning = true;
        m_HasChanges = false;
        m_Thread = std::thread([=]() { WatchFiles(); });
    }

    Filewatcher::~Filewatcher()
    {
        m_IsRunning = false;
        if (m_Thread.joinable())
            m_Thread.join();
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
        while (m_IsRunning)
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