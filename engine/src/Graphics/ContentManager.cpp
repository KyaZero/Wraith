#include "ContentManager.h"

namespace fw
{
	ContentManager* ContentManager::s_Instance = nullptr;

	void ContentManager::Create()
	{
		s_Instance = new ContentManager;
	}

	void ContentManager::Destroy()
	{
		delete s_Instance;
	}

	ContentManager* ContentManager::Get()
	{
		ASSERT_LOG(s_Instance != nullptr, "ContentManager not created before being used.");
		return s_Instance;
	}

	IDType ContentManager::Register(const std::string& path)
	{
		auto fs_path = std::filesystem::path(path);

		if (std::filesystem::exists(fs_path))
		{
			auto hash = std::filesystem::hash_value(path);
			m_Paths[hash] = fs_path;
			return hash;
		}

		return NullId;
	}

	std::optional<std::filesystem::path> ContentManager::GetPath(IDType id)
	{
		auto it = m_Paths.find(id);

		if (it == m_Paths.end())
			return std::nullopt;

		return it->second;
	}
}  // namespace fw
