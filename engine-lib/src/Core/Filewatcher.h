#pragma once
#include "Logger.h"
#include <functional>
#include <filesystem>
#include <unordered_map>
#include <thread>

namespace fw
{
	using WatchCallback = std::function<void()>;

	class Filewatcher
	{
	public:
		struct FileEntry
		{
			WatchCallback callback;
			std::filesystem::file_time_type last_write_time;
		};
		Filewatcher();
		~Filewatcher();

		static Filewatcher* Get();
		static void Create();
		static void Destroy();

		void Watch(const std::string& path, WatchCallback callback);
		void FlushChanges();

	private:
		void WatchFiles();

		std::unordered_map<std::string, FileEntry> m_Entries;
		std::vector<std::string> m_Changes;
		std::thread m_Thread;

		static Filewatcher* m_Instance;

		volatile bool m_IsRunning;
		volatile bool m_HasChanges;
	};
}