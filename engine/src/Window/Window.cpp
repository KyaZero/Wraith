#include "Window.h"
#include <Windows.h>
#include "backends/imgui_impl_win32.h"
#include "Core/Logger.h"
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include "Input/Input.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static WNDPROC currentWndProc;
static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return 0;

	return CallWindowProc(currentWndProc, hWnd, msg, wParam, lParam);
}

namespace fw
{
	std::map<void*, std::function<void(u32, u32)>> Window::s_ResizeCallbacks;

	Window::Window() : m_Handle()
	{
	}

	Window::Window(Vec2u resolution, const std::string& title)
	{
		Create(resolution, title);
	}

	Window::~Window()
	{
		glfwTerminate();
	}

	void Window::Create(Vec2u resolution, const std::string& title)
	{
		if (!glfwInit())
		{
			ASSERT_LOG(false, "Failed to initialize GLFW.");
		}

		m_Resolution = resolution;
		m_CurrentTitle = title;

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		m_Handle = glfwCreateWindow(m_Resolution.x, m_Resolution.y, "2DGE", nullptr, nullptr);
		if (m_Handle == nullptr)
		{
			glfwTerminate();
			ASSERT_LOG(false, "Failed to create window.");
		}

		glfwMakeContextCurrent(m_Handle);
		glfwSetWindowUserPointer(m_Handle, this);
		glfwSetWindowSizeCallback(m_Handle, Window::HandleResize);

		currentWndProc = (WNDPROC)GetWindowLongPtr((HWND)GetPlatformHandle(), GWLP_WNDPROC);
		SetWindowLongPtr((HWND)GetPlatformHandle(), GWLP_WNDPROC, (LONG_PTR)WindowProc);

		Input::SetupInputs(this);
	}

	bool Window::ShouldClose()
	{
		return glfwWindowShouldClose(m_Handle);
	}

	void Window::PollEvents()
	{
		glfwPollEvents();
	}

	void Window::RegisterResizeCallback(void* instance, std::function<void(u32, u32)> callback)
	{
		s_ResizeCallbacks.emplace(instance, callback);
	}

	void Window::UnregisterResizeCallback(void* instance)
	{
		s_ResizeCallbacks.erase(instance);
	}

	const Vec2u& Window::GetSize() const
	{
		return m_Resolution;
	}

	GLFWwindow* Window::GetHandle() const
	{
		return m_Handle;
	}

	void* Window::GetPlatformHandle() const
	{
		return glfwGetWin32Window(m_Handle);
	}

	void Window::SetSize(const Vec2u& size)
	{
		glfwSetWindowSize(m_Handle, size.x, size.y);
	}

	void Window::SetTitle(const std::string& title)
	{
		m_CurrentTitle = title;
		glfwSetWindowTitle(m_Handle, title.c_str());
	}

	const std::string& Window::GetTitle() const
	{
		return m_CurrentTitle;
	}

	void Window::HandleResize(GLFWwindow* handle, int width, int height)
	{
		auto* window = (Window*)glfwGetWindowUserPointer(handle);
		window->m_Resolution = { (u32)width, (u32)height };
		for (auto callback : window->s_ResizeCallbacks)
		{
			callback.second(width, height);
		}
	}
}