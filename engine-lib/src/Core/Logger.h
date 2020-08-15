#pragma once
#include "..\Core\Types.h"

#define VERBOSE_LOG(...) ::fw::Logger::Log(::fw::Logger::Level::Verbose, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);
#define INFO_LOG(...) ::fw::Logger::Log(::fw::Logger::Level::Info, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);
#define WARNING_LOG(...) ::fw::Logger::Log(::fw::Logger::Level::Warning, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);
#define ERROR_LOG(...) ::fw::Logger::Log(::fw::Logger::Level::Error, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);
#define FATAL_LOG(...) ::fw::Logger::Log(::fw::Logger::Level::Fatal, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);

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

		static constexpr i32 WhiteTextColorIndex = 1;
		static constexpr i32 InfoColorIndex = 2;
		static constexpr i32 WarningColorIndex = 3;
		static constexpr i32 ErrorColorIndex = 4;
		static constexpr i32 FatalColorIndex = 5;
		static constexpr i32 DarkTextColorIndex = 7;
		static constexpr i32 ConsoleAlpha = 245;

		static void Create();
		static void Destroy();
		static void SetLevel(Level level);
		static void Log(Level level, const c8* file, u32 line, const c8* function, const c8* format, ...);
		static bool Valid();
	private:
		friend class Allocator;
		Logger();
		~Logger();
		static void InitConsole();
		static const char* GetLevelString(Level level);
		static Logger* m_Instance;
		c8* m_Buffer;
		Level m_Level;
	};
}