#include "ContentManager.h"

namespace Wraith
{
    IDType ContentManager::Register(const std::string& path)
    {
        const auto fs_path = std::filesystem::path(path);

        try
        {
            if (std::filesystem::exists(fs_path))
            {
                const auto hash = std::filesystem::hash_value(path);
                m_Paths[hash] = fs_path;
                return hash;
            }
        }
        catch (const std::exception&)
        { }

        return NullID;
    }

    std::optional<std::filesystem::path> ContentManager::GetPath(IDType id)
    {
        const auto it = m_Paths.find(id);

        if (it == m_Paths.end())
            return std::nullopt;

        return it->second;
    }
}  // namespace Wraith
