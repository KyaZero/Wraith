#pragma once

#include "Core/Math/Vec4.h"
#include "Texture.h"
#include "VkUtil.h"
#include "Window/Window.h"
#include "VkPipeline.h"

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

        vk::UniqueInstance m_Instance;
        vk::UniqueSurfaceKHR m_Surface;
        vk::PhysicalDevice m_PhysicalDevice;
        vk::UniqueDevice m_Device;
        vk::UniqueSwapchainKHR m_SwapChain;
        vk::UniqueSemaphore m_ImageAvailableSemaphore;
        vk::UniqueSemaphore m_RenderFinishedSemaphore;
        vk::UniqueCommandPool m_CommandPool;
        vk::Queue m_GraphicsQueue, m_PresentQueue;
        vk::UniqueRenderPass m_RenderPass;
        
        Pipeline m_Pipeline;

        std::vector<vk::UniqueImageView> m_ImageViews;
        std::vector<vk::UniqueFramebuffer> m_Framebuffers;
        std::vector<vk::UniqueCommandBuffer> m_CommandBuffers;
    };
}  // namespace Wraith