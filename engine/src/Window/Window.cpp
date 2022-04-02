#include "Window.h"

#include <GLFW/glfw3.h>
#include <backends/imgui_impl_win32.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include "Core/Logger.h"
#include "Input/Input.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static WNDPROC currentWndProc;
static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return 0;

    return CallWindowProc(currentWndProc, hWnd, msg, wParam, lParam);
}

namespace Wraith
{
    Window::Window()
        : m_Handle()
    { }

    Window::Window(Vec2u resolution, const std::string& title) { Create(resolution, title); }

    Window::~Window() { glfwTerminate(); }

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
        glfwSetWindowContentScaleCallback(m_Handle, Window::HandleContentScale);

        glfwGetWindowContentScale(m_Handle, &m_ContentScale.x, &m_ContentScale.y);

        currentWndProc = (WNDPROC)GetWindowLongPtr((HWND)GetPlatformHandle(), GWLP_WNDPROC);
        SetWindowLongPtr((HWND)GetPlatformHandle(), GWLP_WNDPROC, (LONG_PTR)WindowProc);

        Input::SetupInputs(this);
    }

    bool Window::ShouldClose() { return glfwWindowShouldClose(m_Handle); }

    void Window::PollEvents() { glfwPollEvents(); }

    void Window::RegisterResizeCallback(Handle handle, ResizeCallback callback) { s_ResizeCallbacks.emplace(handle, callback); }

    void Window::UnregisterResizeCallback(Handle handle) { s_ResizeCallbacks.erase(handle); }

    void Window::RegisterContentScaleCallback(Handle handle, ContentScaleCallback callback) { s_ContentScaleCallbacks.emplace(handle, callback); }

    void Window::UnregisterContentScaleCallback(Handle handle) { s_ContentScaleCallbacks.erase(handle); }

    const Vec2u& Window::GetSize() const { return m_Resolution; }

    const Vec2f& Window::GetContentScale() const { return m_ContentScale; }

    GLFWwindow* Window::GetHandle() const { return m_Handle; }

    void* Window::GetPlatformHandle() const { return glfwGetWin32Window(m_Handle); }

    void Window::SetSize(const Vec2u& size) { glfwSetWindowSize(m_Handle, size.x, size.y); }

    void Window::SetTitle(const std::string& title)
    {
        m_CurrentTitle = title;
        glfwSetWindowTitle(m_Handle, title.c_str());
    }

    const std::string& Window::GetTitle() const { return m_CurrentTitle; }

    void Window::HandleResize(GLFWwindow* handle, int width, int height)
    {
        if (width == 0 || height == 0)
            return;

        auto window = (Window*)glfwGetWindowUserPointer(handle);
        window->m_Resolution = { (u32)width, (u32)height };
        for (const auto& [h, callback] : s_ResizeCallbacks)
        {
            callback(width, height);
        }
    }
    void Window::HandleContentScale(GLFWwindow* handle, float scaleX, float scaleY)
    {
        auto window = (Window*)glfwGetWindowUserPointer(handle);
        window->m_ContentScale = { scaleX, scaleY };
        for (const auto& [h, callback] : s_ContentScaleCallbacks)
        {
            callback(scaleX, scaleY);
        }
    }
}  // namespace Wraith