#include "Logger.h"
#include <filesystem>
#include <iostream>
#include <cstdarg>
#include <fstream>
#include <sstream>

//Simply used to customize console colours a bit
#ifdef _WIN32
#include <Windows.h>
#include <cwchar>
#include <wchar.h>

//library has the 4242 warning which would trigger Warnings As Errors
#pragma warning(disable: 4242)
#include <color.hpp>
#pragma warning(default: 4242)
#endif

namespace fs = ::std::filesystem;

namespace fw
{
	Logger* Logger::s_Instance = nullptr;

	Logger::Logger() : m_Buffer(nullptr), m_Queue(), m_Level((char)Level::All), m_Thread(nullptr), m_ShouldLogToFile(true), m_ShouldPrint(true), m_MultiThreaded(false)
	{

	}

	Logger::~Logger()
	{

	}

	void Logger::Create(bool multiThreaded)
	{
		s_Instance = new Logger;

		//Sufficiently large to hold very long messages
		s_Instance->m_Buffer = new char[8192];

		s_Instance->m_MultiThreaded = multiThreaded;
		if (s_Instance->m_MultiThreaded)
			s_Instance->m_Thread = std::make_unique<std::thread>(Update, s_Instance, std::chrono::milliseconds(16));

		s_Instance->VerifyLogPath();

#ifdef _WIN32
		//Customize Console a little bit
		HWND hwnd = GetConsoleWindow();
		SetConsoleTitleW(L"Engine Console");
		SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
		SetLayeredWindowAttributes(hwnd, 0, 245, LWA_ALPHA);

		CONSOLE_FONT_INFOEX cfi;
		cfi.cbSize = sizeof(cfi);
		cfi.nFont = 0;
		cfi.dwFontSize.X = 0;                   // Width of each character in the font
		cfi.dwFontSize.Y = 18;                  // Height
		cfi.FontFamily = FF_DONTCARE;
		cfi.FontWeight = FW_NORMAL;
		wcscpy_s(cfi.FaceName, _countof(cfi.FaceName), L"Consolas");
		SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
#endif
	}

	void Logger::Destroy()
	{
		s_Instance->m_ShouldPrint = false;

		if (s_Instance->m_MultiThreaded)
			s_Instance->m_Thread->join();

		delete s_Instance;
		s_Instance = nullptr;
	}

	Logger* Logger::Get()
	{
		return s_Instance;
	}

	// Date time stuff taken from here:
	// https://stackoverflow.com/a/46866854
	inline std::string GetCurrentDateTime(std::string s)
	{
		time_t now = time(0);
		struct tm tstruct;
		char buf[80] = { 0 };
		localtime_s(&tstruct, &now);
		if (s == "now")
			strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
		else if (s == "date")
			strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);
		return std::string(buf);
	};

	void Logger::Log(Level level, const char* file, u32 line, const char* function, const char* format, ...)
	{
		if (!s_Instance)
			return;

		if (m_MultiThreaded)
			std::lock_guard<std::mutex> lock(m_QueueMutex);

		if ((char)level < (char)m_Level) return;
		if (level == Level::All || level == Level::Count) return;

		std::string filename = fs::path(file).filename().string();
		std::string func(function);
		if (func.find("lambda") != std::string::npos)
			func = func.substr(0, func.find("lambda") + 6) + ">";

		if (u64 pos = func.find_last_of(":"); pos != std::string::npos)
		{
			func = func.substr(pos + 1);
		}

		va_list args;
		va_start(args, format);
		vsprintf_s(m_Buffer, 8192, format, args);
		va_end(args);

		if (m_MultiThreaded)
		{
			m_Queue.push(LogEntry{ filename,func,m_Buffer,line,level });
		}
		else
		{
			Print(s_Instance, LogEntry{ filename,func,m_Buffer,line,level });
		}

		//Force log thread to finish
		if (level == Level::Fatal && m_MultiThreaded)
			m_Thread->join();
	}

#ifdef _WIN32
	auto Logger::GetLevelString(Level level)
	{
		switch (level)
		{
		case Logger::Level::Verbose:
			return dye::white("VERBOSE");
			break;
		case Logger::Level::Info:
			return dye::light_green("INFO");
			break;
		case Logger::Level::Warning:
			return dye::light_yellow("WARNING");
			break;
		case Logger::Level::Error:
			return dye::light_red("ERROR");
			break;
		case Logger::Level::Fatal:
			return dye::light_purple("FATAL");
			break;
		}
		return dye::black("");
	}
#else
	std::string Logger::GetLevelString(Level level)
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
		}
		return "";
	}
#endif

	void Logger::VerifyLogPath()
	{
		//Make sure the saved directory exists so that we can create a log file
		if (!fs::is_directory("saved") || !fs::exists("saved"))
			fs::create_directory("saved");

		//Remove the file if it already exists a log for this date
		m_LogPath = "saved/log_" + GetCurrentDateTime("date") + ".txt";
		if (fs::exists(m_LogPath))
			fs::remove(m_LogPath);

		//Create the file
		std::ofstream ofs(m_LogPath.c_str(), std::ios_base::out);
	}

	void Logger::LogToFile(const std::string& msg)
	{
		std::string now = GetCurrentDateTime("now");
		std::ofstream ofs(m_LogPath.c_str(), std::ios_base::out | std::ios_base::app);
		ofs << "[" << now << "]\t" << msg;
		ofs.close();
	}

	void Logger::Update(Logger* instance, std::chrono::duration<double, std::milli> interval)
	{
		while (instance->m_ShouldPrint || !instance->m_Queue.empty())
		{
			auto t1 = std::chrono::steady_clock::now();
			{
				std::lock_guard<std::mutex> lock(instance->m_QueueMutex);
				while (!instance->m_Queue.empty())
				{
					auto& entry = instance->m_Queue.front();

					Print(instance, entry);
					instance->m_Queue.pop();
				}
			}
			auto t2 = std::chrono::steady_clock::now();
			std::chrono::duration<double, std::milli> time_took = t2 - t1;

			if (time_took < interval)
				std::this_thread::sleep_for(interval - time_took);
		}
	}

	void Logger::Print(Logger* instance, const LogEntry& entry)
	{
#ifdef _WIN32
		auto dyed = dye::grey("[") + GetLevelString(entry.level) + dye::grey("]") + " " + dye::grey(entry.filename + ":" + std::to_string(entry.line) + ":" + entry.function + ": ") + dye::bright_white(entry.msg) + "\n";
		std::cout << dyed;

		std::stringstream sstr;
		sstr << dyed;

		if (instance->m_ShouldLogToFile)
			instance->LogToFile(sstr.str());
#else
		auto log = "[" + GetLevelString(entry.level) + "]" + " " + entry.filename + ":" + std::to_string(entry.line) + ":" + entry.function + ": " + entry.msg;
		puts(log.c_str());

		if (instance->m_ShouldLogToFile)
			instance->LogToFile(log);
#endif
	}

	void Logger::SetLevel(Level level)
	{
		Get()->m_Level = (char)level;
	}

	void Logger::SetPrint(bool shouldPrint)
	{
		Get()->m_ShouldPrint = shouldPrint;
	}

	void Logger::SetShouldLogToFile(bool shouldLogToFile)
	{
		Get()->m_ShouldLogToFile = shouldLogToFile;
	}
}