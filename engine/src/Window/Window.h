#pragma once

#include <functional>

struct GLFWwindow;

namespace fw
{
    using Handle = void*;
    using ResizeCallback = std::function<void(u32, u32)>;

    class Window
    {
    public:
        Window();
        Window(Vec2u resolution, const std::string& title);
        ~Window();

        void Create(Vec2u resolution, const std::string& title);

        bool ShouldClose();
        void PollEvents();

        static void RegisterResizeCallback(void* instance, ResizeCallback callback);
        static void UnregisterResizeCallback(void* instance);

        const Vec2u& GetSize() const;

        GLFWwindow* GetHandle() const;
        void* GetPlatformHandle() const;

        void SetSize(const Vec2u& size);
        void SetTitle(const std::string& title);

        const std::string& GetTitle() const;

    private:
        static void HandleResize(GLFWwindow* window, int width, int height);

        Vec2u m_Resolution;

        GLFWwindow* m_Handle;

        std::string m_CurrentTitle;
        static std::unordered_map<Handle, ResizeCallback> s_ResizeCallbacks;
    };
}  // namespace fw