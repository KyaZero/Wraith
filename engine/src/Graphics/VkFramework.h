#pragma once

#include "Core/Math/Vec4.h"
#include "Texture.h"
#include "Window/Window.h"

namespace Wraith
{
    class VkFramework
    {
    public:
        VkFramework(Window& window);
        ~VkFramework();

        bool Init();

        void BeginFrame(const Vec4f& clear_color);
        void EndFrame();

        static void BeginEvent(std::string name);
        static void EndEvent();

    private:
        void ResizeBackbuffer(u32 width, u32 height);

        Window& m_Window;
        struct Data;
        std::unique_ptr<Data> m_Data;
    };
}  // namespace Wraith