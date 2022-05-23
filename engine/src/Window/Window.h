#pragma once

#include <functional>

struct GLFWwindow;

namespace Wraith
{
    using Handle = void*;
    using ResizeCallback = std::function<void(u32, u32)>;
    using ContentScaleCallback = std::function<void(f32, f32)>;

    class Window
    {
    public:
        Window();
        Window(Vec2u resolution, const std::string& title);
        ~Window();

        void Create(Vec2u resolution, const std::string& title);

        bool ShouldClose();
        void PollEvents();

        static void RegisterResizeCallback(Handle handle, ResizeCallback callback);
        static void UnregisterResizeCallback(Handle handle);
        static void RegisterContentScaleCallback(Handle handle, ContentScaleCallback callback);
        static void UnregisterContentScaleCallback(Handle handle);

        const Vec2u& GetSize() const;
        const Vec2f& GetContentScale() const;

        GLFWwindow* GetHandle() const;
        void* GetPlatformHandle() const;

        void SetSize(const Vec2u& size);
        void SetTitle(const std::string& title);

        const std::string& GetTitle() const;

        void ResetResizedFlag() { m_FramebufferResized = false; }
        bool WasResized() const { return m_FramebufferResized; }

    private:
        static void HandleResize(GLFWwindow* handle, int width, int height);
        static void HandleContentScale(GLFWwindow* handle, float scaleX, float scaleY);

        Vec2u m_Resolution;
        Vec2f m_ContentScale;
        bool m_FramebufferResized;

        GLFWwindow* m_Handle;

        std::string m_CurrentTitle;
        static inline std::unordered_map<Handle, ResizeCallback> s_ResizeCallbacks;
        static inline std::unordered_map<Handle, ContentScaleCallback> s_ContentScaleCallbacks;
    };
}  // namespace Wraith