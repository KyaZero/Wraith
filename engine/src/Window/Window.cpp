#include "Window.h"
#include <Windows.h>
#include "WindowStyle.h"
#include "Mouse.h"
#include "examples/imgui_impl_win32.h"
#include "Core/Logger.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
namespace fw
{
	static LRESULT CALLBACK WindowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

	Window::Window() :
		m_Handle(NULL),
		m_Fullscreen(0),
		m_KeyRepeatEnabled(false),
		m_IsOpen(false),
		m_MouseInside(false),
		m_CursorGrabbed(false),
		m_CursorVisible(true),
		m_LastSize(0, 0),
		m_Resizing(false)
	{

	}

	Window::Window(VideoMode mode, const std::wstring& title, u32 style) :
		m_Handle(NULL),
		m_Fullscreen((style& Style::Fullscreen) != 0),
		m_KeyRepeatEnabled(false),
		m_IsOpen(true),
		m_MouseInside(false),
		m_CursorGrabbed(false),
		m_CursorVisible(true),
		m_LastSize(mode.width, mode.height),
		m_Resizing(false)
	{
		Create(mode, title, style);
	}

	Window::~Window()
	{
		SetCursorVisible(true);
		SetTracking(false);
		ReleaseCapture();
	}

	void Window::Create(VideoMode mode, const std::wstring& title, u32 style)
	{
		m_IsOpen = true;
		m_Fullscreen = (style & Style::Fullscreen) != 0;

		LPCWSTR class_name = L"Engine_Class";

		WNDCLASSW window_class;
		window_class.style = 0;
		window_class.lpfnWndProc = &WindowProc;
		window_class.cbClsExtra = 0;
		window_class.cbWndExtra = 0;
		window_class.hInstance = GetModuleHandleW(NULL);
		window_class.hIcon = NULL;
		window_class.hCursor = 0;
		window_class.hbrBackground = 0;
		window_class.lpszMenuName = NULL;
		window_class.lpszClassName = class_name;
		RegisterClassW(&window_class);

		HDC screen_dc = GetDC(NULL);
		i32 left = (GetDeviceCaps(screen_dc, HORZRES) - (i32)mode.width) / 2;
		i32 top = (GetDeviceCaps(screen_dc, VERTRES) - (i32)mode.height) / 2;
		i32 width = mode.width;
		i32 height = mode.height;
		ReleaseDC(NULL, screen_dc);

		DWORD win32_style = WS_VISIBLE;
		if (style == Style::None)
		{
			win32_style |= WS_POPUP;
		}
		else
		{
			if (style & Style::Titlebar) win32_style |= WS_CAPTION;
			if (style & Style::Resize) win32_style |= WS_THICKFRAME | WS_MAXIMIZEBOX;
			if (style & Style::Close) win32_style |= WS_SYSMENU;
			if (style & Style::Minimize) win32_style |= WS_MINIMIZEBOX;
		}

		if (!m_Fullscreen)
		{
			RECT rectangle = { 0, 0, width, height };
			AdjustWindowRect(&rectangle, win32_style, false);
			width = rectangle.right - rectangle.left;
			height = rectangle.bottom - rectangle.top;
		}

		m_Handle = CreateWindowW(class_name, title.c_str(), win32_style, left, top, width, height, NULL, NULL, GetModuleHandle(NULL), this);

		SetSize(Vec2u(mode.width, mode.height));
		m_LastSize = { mode.width, mode.height };

		if (m_Fullscreen)
			SwitchToFullscreen(mode);
	}

	void Window::Close()
	{
		m_IsOpen = false;
	}

	WindowHandle Window::GetHandle() const
	{
		return m_Handle;
	}

	bool Window::IsOpen() const
	{
		return m_IsOpen;
	}

	bool Window::PollEvent(Event& event)
	{
		if (PopEvent(event))
		{
			return FilterEvent(event);
		}
		else
		{
			return false;
		}
	}

	void Window::ProcessEvent(u32 message, i64 wParam, i64 lParam)
	{
		if (m_Handle == nullptr)
			return;

		switch (message)
		{
		case WM_DESTROY:
			Cleanup();
			break;

		case WM_CLOSE:
		{
			Event event;
			event.type = Event::Closed;
			PushEvent(event);
			break;
		}

		case WM_SETCURSOR:
		{
			if (LOWORD(lParam) == HTCLIENT)
			{
				SetCursor(m_CursorVisible ? (HCURSOR)m_LastCursor : NULL);
			}

			break;
		}

		case WM_SIZE:
		{
			if (wParam != SIZE_MINIMIZED && !m_Resizing && m_LastSize != GetSize())
			{
				m_LastSize = GetSize();

				Event event;
				event.type = Event::Resized;
				event.size.width = m_LastSize.x;
				event.size.height = m_LastSize.y;
				PushEvent(event);

				GrabCursor(m_CursorGrabbed);
			}
			break;
		}

		case WM_ENTERSIZEMOVE:
		{
			m_Resizing = true;
			GrabCursor(m_CursorGrabbed);
			break;
		}

		case WM_EXITSIZEMOVE:
		{
			m_Resizing = false;

			if (m_LastSize != GetSize())
			{
				m_LastSize = GetSize();

				Event event;
				event.type = Event::Resized;
				event.size.width = m_LastSize.x;
				event.size.height = m_LastSize.y;
				PushEvent(event);
			}
			GrabCursor(m_CursorGrabbed);
			break;
		}

		case WM_SETFOCUS:
		{
			GrabCursor(m_CursorGrabbed);
			Event event;
			event.type = Event::GainedFocus;
			PushEvent(event);
			break;
		}

		case WM_KILLFOCUS:
		{
			GrabCursor(m_CursorGrabbed);

			Event event;
			event.type = Event::LostFocus;
			PushEvent(event);
			break;
		}

		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		{
			if (m_KeyRepeatEnabled || ((HIWORD(lParam) & KF_REPEAT) == 0))
			{
				Event event;
				event.type = Event::KeyPressed;
				event.key.alt = HIWORD(GetKeyState(VK_MENU) != 0);
				event.key.control = HIWORD(GetKeyState(VK_CONTROL) != 0);
				event.key.shift = HIWORD(GetKeyState(VK_SHIFT) != 0);
				event.key.system = HIWORD(GetKeyState(VK_LWIN)) || HIWORD(GetKeyState(VK_RWIN));
				event.key.code = VKeyCodeToEngineKey(wParam, lParam);
				PushEvent(event);
			}
		}

		case WM_KEYUP:
		case WM_SYSKEYUP:
		{
			Event event;
			event.type = Event::KeyReleased;
			event.key.alt = HIWORD(GetKeyState(VK_MENU) != 0);
			event.key.control = HIWORD(GetKeyState(VK_CONTROL) != 0);
			event.key.shift = HIWORD(GetKeyState(VK_SHIFT) != 0);
			event.key.system = HIWORD(GetKeyState(VK_LWIN)) || HIWORD(GetKeyState(VK_RWIN));
			event.key.code = VKeyCodeToEngineKey(wParam, lParam);
			PushEvent(event);
		}

		case WM_MOUSEWHEEL:
		{
			POINT position;
			position.x = (i16)(LOWORD(lParam));
			position.y = (i16)(HIWORD(lParam));
			ScreenToClient((HWND)m_Handle, &position);

			i16 delta = (i16)(HIWORD(wParam));
			Event event;
			event.type = Event::MouseWheelMoved;
			event.mousewheel.delta = delta / 120;
			event.mousewheel.x = position.x;
			event.mousewheel.y = position.y;
			PushEvent(event);

			event.type = Event::MouseWheelScrolled;
			event.mousewheel_scroll.wheel = Mouse::VerticalWheel;
			event.mousewheel_scroll.delta = (f32)delta / 120.0f;
			event.mousewheel_scroll.x = position.x;
			event.mousewheel_scroll.y = position.y;
			PushEvent(event);
			break;
		}

		case WM_MOUSEHWHEEL:
		{
			POINT position;
			position.x = (i16)(LOWORD(lParam));
			position.y = (i16)(LOWORD(lParam));
			ScreenToClient((HWND)m_Handle, &position);

			i16 delta = (i16)(HIWORD(wParam));

			Event event;
			event.type = Event::MouseWheelScrolled;
			event.mousewheel_scroll.wheel = Mouse::HorizontalWheel;
			event.mousewheel_scroll.delta = -(f32)delta / 120.0f;
			event.mousewheel_scroll.x = position.x;
			event.mousewheel_scroll.y = position.y;
			PushEvent(event);
			break;
		}

		case WM_LBUTTONDOWN:
		{
			Event event;
			event.type = Event::MouseButtonPressed;
			event.mouse_button.button = Mouse::Left;
			event.mouse_button.x = (i16)(LOWORD(lParam));
			event.mouse_button.y = (i16)(HIWORD(lParam));
			PushEvent(event);
			break;
		}

		case WM_LBUTTONUP:
		{
			Event event;
			event.type = Event::MouseButtonReleased;
			event.mouse_button.button = Mouse::Left;
			event.mouse_button.x = (i16)(LOWORD(lParam));
			event.mouse_button.y = (i16)(HIWORD(lParam));
			PushEvent(event);
			break;
		}

		case WM_RBUTTONDOWN:
		{
			Event event;
			event.type = Event::MouseButtonPressed;
			event.mouse_button.button = Mouse::Right;
			event.mouse_button.x = (i16)(LOWORD(lParam));
			event.mouse_button.y = (i16)(HIWORD(lParam));
			PushEvent(event);
			break;
		}

		case WM_RBUTTONUP:
		{
			Event event;
			event.type = Event::MouseButtonReleased;
			event.mouse_button.button = Mouse::Right;
			event.mouse_button.x = (i16)(LOWORD(lParam));
			event.mouse_button.y = (i16)(HIWORD(lParam));
			PushEvent(event);
			break;
		}

		case WM_MBUTTONDOWN:
		{
			Event event;
			event.type = Event::MouseButtonPressed;
			event.mouse_button.button = Mouse::Middle;
			event.mouse_button.x = (i16)(LOWORD(lParam));
			event.mouse_button.y = (i16)(HIWORD(lParam));
			PushEvent(event);
			break;
		}

		case WM_MBUTTONUP:
		{
			Event event;
			event.type = Event::MouseButtonReleased;
			event.mouse_button.button = Mouse::Middle;
			event.mouse_button.x = (i16)(LOWORD(lParam));
			event.mouse_button.y = (i16)(HIWORD(lParam));
			PushEvent(event);
			break;
		}

		case WM_MOUSELEAVE:
		{
			if (m_MouseInside)
			{
				m_MouseInside = false;

				Event event;
				event.type = Event::MouseLeave;
				PushEvent(event);
			}
			break;
		}

		case WM_MOUSEMOVE:
		{
			i32 x = (i16)(LOWORD(lParam));
			i32 y = (i16)(HIWORD(lParam));

			RECT area;
			GetClientRect((HWND)m_Handle, &area);

			if ((wParam & (MK_LBUTTON | MK_MBUTTON | MK_RBUTTON | MK_XBUTTON1 | MK_XBUTTON2)) == 0)
			{
				if (GetCapture() == (HWND)m_Handle)
					ReleaseCapture();
			}
			else if (GetCapture() != (HWND)m_Handle)
			{
				SetCapture((HWND)m_Handle);
			}

			if ((x < area.left) || (x > area.right) || (y < area.top) || (y > area.bottom))
			{
				if (m_MouseInside)
				{
					m_MouseInside = false;

					SetTracking(false);

					Event event;
					event.type = Event::MouseLeave;
					PushEvent(event);
				}
			}
			else
			{
				if (!m_MouseInside)
				{
					m_MouseInside = true;

					SetTracking(true);

					Event event;
					event.type = Event::MouseEnter;
					PushEvent(event);
				}
			}

			Event event;
			event.type = Event::MouseMove;
			event.mouse_move.x = x;
			event.mouse_move.y = y;
			PushEvent(event);
			break;
		}

		default:
			break;
		}
	}

	void Window::Cleanup()
	{
		if (m_Fullscreen)
		{
			ChangeDisplaySettingsW(NULL, 0);
			m_Fullscreen = false;
		}
	}

	void Window::SetKeyRepeatEnabled(bool enabled)
	{
		m_KeyRepeatEnabled = enabled;
	}

	void Window::SetCursorGrabbed(bool grabbed)
	{
		m_CursorGrabbed = grabbed;
		GrabCursor(m_CursorGrabbed);
	}

	void Window::SetCursorVisible(bool visible)
	{
		m_CursorVisible = visible;
		SetCursor(m_CursorVisible ? (HCURSOR)m_LastCursor : NULL);
	}

	void Window::SetVisible(bool visible)
	{
		ShowWindow((HWND)m_Handle, visible ? SW_SHOW : SW_HIDE);
	}

	void Window::RequestFocus()
	{
		DWORD this_pid = GetWindowThreadProcessId((HWND)m_Handle, NULL);
		DWORD foreground_pid = GetWindowThreadProcessId(GetForegroundWindow(), NULL);

		if (this_pid == foreground_pid)
		{
			SetForegroundWindow((HWND)m_Handle);
		}
		else
		{
			FLASHWINFO info;
			info.cbSize = sizeof(info);
			info.hwnd = (HWND)m_Handle;
			info.dwFlags = FLASHW_TRAY;
			info.dwTimeout = 0;
			info.uCount = 3;

			FlashWindowEx(&info);
		}
	}

	bool Window::HasFocus() const
	{
		return m_Handle == GetForegroundWindow();
	}

	void Window::Subscribe(Event::EventType type, std::function<void(const Event&)> callback)
	{
		if (m_Listeners.find(type) == m_Listeners.end())
		{
			m_Listeners[type] = { };
		}
		m_Listeners[type].push_back(callback);
	}

	Vec2u Window::GetSize() const
	{
		RECT rect;
		GetClientRect((HWND)m_Handle, &rect);

		return Vec2u(rect.right - rect.left, rect.bottom - rect.top);
	}

	Vec2f Window::GetSizef() const
	{
		auto& size = GetSize();
		return { (f32)size.x, (f32)size.y };
	}

	f32 Window::GetAspectRatio() const
	{
		return m_Size.x / m_Size.y;
	}

	void Window::ProcessEvents()
	{
		MSG message = { 0 };
		while (PeekMessageW(&message, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessageW(&message);
		}
	}

	void Window::SetSize(const Vec2u& size)
	{
		RECT rectangle = { 0 , 0, (i64)size.x, (i64)size.y };
		AdjustWindowRect(&rectangle, GetWindowLong((HWND)m_Handle, GWL_STYLE), false);
		i32 width = rectangle.right - rectangle.left;
		i32 height = rectangle.bottom - rectangle.top;

		SetWindowPos((HWND)m_Handle, nullptr, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER);
	}

	void Window::SetTitle(const std::string& title)
	{
		SetWindowText((HWND)m_Handle, title.c_str());
	}

	void Window::SwitchToFullscreen(const VideoMode& mode)
	{
		DEVMODEW dev_mode;
		dev_mode.dmSize = sizeof(dev_mode);
		dev_mode.dmPelsWidth = mode.width;
		dev_mode.dmPelsHeight = mode.height;
		dev_mode.dmBitsPerPel = mode.bpp;
		dev_mode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

		if (ChangeDisplaySettingsW(&dev_mode, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			ERROR_LOG("Failed to change display mode for fullscreen");
			return;
		}

		SetWindowLongW((HWND)m_Handle, GWL_STYLE, WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
		SetWindowLongW((HWND)m_Handle, GWL_EXSTYLE, WS_EX_APPWINDOW);

		SetWindowPos((HWND)m_Handle, HWND_TOP, 0, 0, mode.width, mode.height, SWP_FRAMECHANGED);
		ShowWindow((HWND)m_Handle, SW_SHOW);
	}

	void Window::SetTracking(bool track)
	{
		TRACKMOUSEEVENT mouse_event;
		mouse_event.cbSize = sizeof(TRACKMOUSEEVENT);
		mouse_event.dwFlags = track ? TME_LEAVE : TME_CANCEL;
		mouse_event.hwndTrack = (HWND)m_Handle;
		mouse_event.dwHoverTime = HOVER_DEFAULT;
		TrackMouseEvent(&mouse_event);
	}

	void Window::GrabCursor(bool grabbed)
	{
		if (grabbed)
		{
			RECT rect;
			GetClientRect((HWND)m_Handle, &rect);
			MapWindowPoints((HWND)m_Handle, NULL, (LPPOINT)(&rect), 2);
			ClipCursor(&rect);
		}
		else
		{
			ClipCursor(NULL);
		}
	}

	Key Window::VKeyCodeToEngineKey(i64 key, i64 flags)
	{
		switch (key)
		{
		case VK_SHIFT:
		{
			static UINT left_shift = MapVirtualKeyW(VK_LSHIFT, MAPVK_VK_TO_VSC);
			UINT scancode = (UINT)((flags & (0xFF << 16)) >> 16);
			return scancode == left_shift ? Key::LShift : Key::RShift;
		}

		case VK_MENU:		return (HIWORD(flags) & KF_EXTENDED) ? Key::RAlt : Key::LAlt;
		case VK_CONTROL:	return (HIWORD(flags) & KF_EXTENDED) ? Key::RControl : Key::LControl;

		case VK_LWIN:       return Key::LSystem;
		case VK_RWIN:       return Key::RSystem;
		case VK_APPS:       return Key::Menu;
		case VK_OEM_1:      return Key::Semicolon;
		case VK_OEM_2:      return Key::Slash;
		case VK_OEM_PLUS:   return Key::Equal;
		case VK_OEM_MINUS:  return Key::Hyphen;
		case VK_OEM_4:      return Key::LBracket;
		case VK_OEM_6:      return Key::RBracket;
		case VK_OEM_COMMA:  return Key::Comma;
		case VK_OEM_PERIOD: return Key::Period;
		case VK_OEM_7:      return Key::Quote;
		case VK_OEM_5:      return Key::Backslash;
		case VK_OEM_3:      return Key::Tilde;
		case VK_ESCAPE:     return Key::Escape;
		case VK_SPACE:      return Key::Space;
		case VK_RETURN:     return Key::Enter;
		case VK_BACK:       return Key::Backspace;
		case VK_TAB:        return Key::Tab;
		case VK_PRIOR:      return Key::PageUp;
		case VK_NEXT:       return Key::PageDown;
		case VK_END:        return Key::End;
		case VK_HOME:       return Key::Home;
		case VK_INSERT:     return Key::Insert;
		case VK_DELETE:     return Key::Delete;
		case VK_ADD:        return Key::Add;
		case VK_SUBTRACT:   return Key::Subtract;
		case VK_MULTIPLY:   return Key::Multiply;
		case VK_DIVIDE:     return Key::Divide;
		case VK_PAUSE:      return Key::Pause;
		case VK_F1:         return Key::F1;
		case VK_F2:         return Key::F2;
		case VK_F3:         return Key::F3;
		case VK_F4:         return Key::F4;
		case VK_F5:         return Key::F5;
		case VK_F6:         return Key::F6;
		case VK_F7:         return Key::F7;
		case VK_F8:         return Key::F8;
		case VK_F9:         return Key::F9;
		case VK_F10:        return Key::F10;
		case VK_F11:        return Key::F11;
		case VK_F12:        return Key::F12;
		case VK_F13:        return Key::F13;
		case VK_F14:        return Key::F14;
		case VK_F15:        return Key::F15;
		case VK_LEFT:       return Key::Left;
		case VK_RIGHT:      return Key::Right;
		case VK_UP:         return Key::Up;
		case VK_DOWN:       return Key::Down;
		case VK_NUMPAD0:    return Key::Numpad0;
		case VK_NUMPAD1:    return Key::Numpad1;
		case VK_NUMPAD2:    return Key::Numpad2;
		case VK_NUMPAD3:    return Key::Numpad3;
		case VK_NUMPAD4:    return Key::Numpad4;
		case VK_NUMPAD5:    return Key::Numpad5;
		case VK_NUMPAD6:    return Key::Numpad6;
		case VK_NUMPAD7:    return Key::Numpad7;
		case VK_NUMPAD8:    return Key::Numpad8;
		case VK_NUMPAD9:    return Key::Numpad9;
		case 'A':           return Key::A;
		case 'Z':           return Key::Z;
		case 'E':           return Key::E;
		case 'R':           return Key::R;
		case 'T':           return Key::T;
		case 'Y':           return Key::Y;
		case 'U':           return Key::U;
		case 'I':           return Key::I;
		case 'O':           return Key::O;
		case 'P':           return Key::P;
		case 'Q':           return Key::Q;
		case 'S':           return Key::S;
		case 'D':           return Key::D;
		case 'F':           return Key::F;
		case 'G':           return Key::G;
		case 'H':           return Key::H;
		case 'J':           return Key::J;
		case 'K':           return Key::K;
		case 'L':           return Key::L;
		case 'M':           return Key::M;
		case 'W':           return Key::W;
		case 'X':           return Key::X;
		case 'C':           return Key::C;
		case 'V':           return Key::V;
		case 'B':           return Key::B;
		case 'N':           return Key::N;
		case '0':           return Key::Num0;
		case '1':           return Key::Num1;
		case '2':           return Key::Num2;
		case '3':           return Key::Num3;
		case '4':           return Key::Num4;
		case '5':           return Key::Num5;
		case '6':           return Key::Num6;
		case '7':           return Key::Num7;
		case '8':           return Key::Num8;
		case '9':           return Key::Num9;
		}

		return Key::Unknown;
	}

	bool Window::FilterEvent(const Event& event)
	{
		if (event.type == Event::Resized)
		{
			//Cache the new size
			m_Size.x = event.size.width;
			m_Size.y = event.size.height;
		}
		return true;
	}

	void Window::PushEvent(const Event& event)
	{
		m_Events.push(event);
	}

	bool Window::PopEvent(Event& event)
	{
		if (m_Events.empty())
		{
			ProcessEvents();
		}

		if (!m_Events.empty())
		{
			event = m_Events.front();
			m_Events.pop();

			if (auto it = m_Listeners.find(event.type); it != m_Listeners.end())
			{
				for (auto& callback : it->second)
					callback(event);
			}

			return true;
		}

		return false;
	}

	LRESULT CALLBACK WindowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (ImGui_ImplWin32_WndProcHandler(handle, message, wParam, lParam))
			return 0;

		if (message == WM_CREATE)
		{
			LONG_PTR window = (LONG_PTR)reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams;

			SetWindowLongPtrW(handle, GWLP_USERDATA, window);
		}

		Window* window = handle ? (Window*)(GetWindowLongPtr(handle, GWLP_USERDATA)) : nullptr;

		if (window)
		{
			window->ProcessEvent(message, wParam, lParam);
		}

		return DefWindowProcW(handle, message, wParam, lParam);
	}
}