#pragma once
#include "Event.h"
#include "VideoMode.h"
#include "WindowStyle.h"
#include "..\Core\Math\Vec2.h"
#include <queue>
#include <functional>

namespace fw
{
	using Handle = void*;
	using WindowHandle = Handle;

	class Window
	{
	public:
		Window();
		Window(VideoMode mode, const std::wstring& title, u32 style);
		~Window();

		void Create(VideoMode mode, const std::wstring& title, u32 style);
		void Close();

		WindowHandle GetHandle() const;

		bool IsOpen() const;
		bool HasFocus() const;
		bool PollEvent(Event& event);

		void ProcessEvent(u32 message, i64 wParam, i64 lParam);
		void Cleanup();
		void SetSize(const Vec2u& size);
		void SetTitle(const std::string& title);

		void SetKeyRepeatEnabled(bool enabled);
		void SetCursorGrabbed(bool grabbed);
		void SetCursorVisible(bool visible);
		void SetVisible(bool visible);
		void RequestFocus();

		void Subscribe(Event::EventType type, std::function<void(const Event&)> callback);

		Vec2u GetSize() const;

	private:
		Key VKeyCodeToEngineKey(i64 key, i64 flags);

		bool FilterEvent(const Event& event);
		void PushEvent(const Event& event);
		bool PopEvent(Event& event);
		void ProcessEvents();
		void SwitchToFullscreen(const VideoMode& mode);
		void SetTracking(bool track);
		void GrabCursor(bool grabbed);

		std::queue<Event> m_Events;

		Vec2u m_Size;
		Vec2u m_LastSize;

		WindowHandle m_Handle;
		bool m_IsOpen, m_Fullscreen, m_Resizing, m_KeyRepeatEnabled, m_MouseInside, m_CursorVisible, m_CursorGrabbed;
		Handle m_LastCursor;

		std::unordered_map<Event::EventType, std::vector<std::function<void(const Event&)>>> m_Listeners;
	};
}