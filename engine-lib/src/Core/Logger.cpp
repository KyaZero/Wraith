#include "Logger.h"
#include <Windows.h>
#include <filesystem>
#include <iostream>
#include <cstdarg>

namespace fw
{
	namespace fs = ::std::filesystem;

	Logger* Logger::m_Instance = nullptr;

	void Logger::Create()
	{
		if (!m_Instance)
		{
			m_Instance = new Logger;
			m_Instance->InitConsole();
			m_Instance->m_Buffer = new c8[8192];
		}
	}

	void Logger::Destroy()
	{
		if (m_Instance && m_Instance->m_Buffer)
			delete m_Instance->m_Buffer;

		if (m_Instance)
			delete m_Instance;

		m_Instance = nullptr;
	}

	void Logger::SetLevel(Level level)
	{
		m_Instance->m_Level = level;
	}

	void Logger::Log(Level level, const c8* file, u32 line, const c8* function, const c8* format, ...)
	{
		if (!m_Instance)
		{
			return;
		}

		if ((c8)level < (c8)m_Instance->m_Level) return;
		if (level == Level::All || level == Level::Count) return;

		std::string filename = fs::path(file).filename().string();

		std::string func(function);
		if (func.find("lambda") != std::string::npos) func = func.substr(0, func.find("lambda") + 6) + ">";

		if (u64 pos = func.find_last_of(":"); pos != std::string::npos)
		{
			func = func.substr(pos + 1);
		}

		c8* buffer = m_Instance->m_Buffer;

		va_list args;
		va_start(args, format);
		vsprintf_s(buffer, 8192, format, args);
		va_end(args);

		HWND hwnd = GetConsoleWindow();
		if (level >= Level::Error)
		{
			FlashWindow(hwnd, false);
		}

		HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(console, DarkTextColorIndex);
		printf("[");
		SetConsoleTextAttribute(console, (c8)level);
		printf("%s", GetLevelString(level));
		SetConsoleTextAttribute(console, DarkTextColorIndex);
		printf("] %s:%d:%s: ", filename.c_str(), line, func.c_str());
		SetConsoleTextAttribute(console, WhiteTextColorIndex);
		printf("%s\n", buffer);
	}

	bool Logger::Valid()
	{
		return m_Instance && m_Instance->m_Buffer;
	}

	Logger::Logger() : m_Buffer(nullptr), m_Level(Level::All)
	{
	}

	Logger::~Logger()
	{
	}

	void Logger::InitConsole()
	{
#ifdef _RETAIL
		return;
#else
#pragma warning( push )
#pragma warning( disable : 4996 )
		AllocConsole();
		FILE* f = nullptr;
		f = freopen("CONIN$", "r", stdin);
		f = freopen("CONOUT$", "w", stdout);
		f = freopen("CONOUT$", "w", stderr);

		setbuf(stdin, NULL);
		setbuf(stdout, NULL);
		setbuf(stderr, NULL);
#pragma warning( pop )

		HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

		_CONSOLE_SCREEN_BUFFER_INFOEX csbi;
		csbi.cbSize = sizeof(_CONSOLE_SCREEN_BUFFER_INFOEX);
		GetConsoleScreenBufferInfoEx(console, &csbi);

		csbi.ColorTable[0] = (25 << 0) | (25 << 8) | (30 << 16);
		csbi.ColorTable[DarkTextColorIndex] = (150 << 0) | (150 << 8) | (150 << 16);
		csbi.ColorTable[InfoColorIndex] = (0 << 0) | (150 << 8) | (75 << 16);
		csbi.ColorTable[WarningColorIndex] = (200 << 0) | (200 << 8) | (50 << 16);
		csbi.ColorTable[ErrorColorIndex] = (255 << 0) | (50 << 8) | (50 << 16);
		csbi.ColorTable[WhiteTextColorIndex] = (219 << 0) | (221 << 8) | (231 << 16);
		csbi.cbSize = sizeof(csbi);
		SetConsoleScreenBufferInfoEx(console, &csbi);

		HWND hwnd = GetConsoleWindow();

		SetConsoleTitleW(L"Engine Console");
		SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
		SetLayeredWindowAttributes(hwnd, 0, ConsoleAlpha, LWA_ALPHA);

		MoveWindow(hwnd, 0, 0, 1250, 600, true);
#endif
	}

	const char* Logger::GetLevelString(Level level)
	{
		switch (level)
		{
		case Logger::Level::Verbose:
			return "VERBOSE";
			break;
		case Logger::Level::Info:
			return "INFO";
			break;
		case Logger::Level::Warning:
			return "WARNING";
			break;
		case Logger::Level::Error:
			return "ERROR";
			break;
		case Logger::Level::Fatal:
			return "FATAL";
			break;
		default:
			return "???";
			break;
		}
	}
}