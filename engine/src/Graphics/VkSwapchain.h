#pragma once

#include "VkDevice.h"
#include "VkUtil.h"

namespace Wraith
{
    class SwapChain
    {
    public:
        SwapChain(Device& device, vk::Extent2D windowExtent);
        SwapChain(Device& device, vk::Extent2D windowExtent, std::shared_ptr<SwapChain> previous);
        SwapChain(const SwapChain&) = delete;
        SwapChain& operator=(const SwapChain&) = delete;

        vk::Framebuffer GetFrameBuffer(int index) { return *m_Framebuffers[index]; }
        vk::RenderPass GetRenderPass() { return *m_RenderPass; }
        vk::ImageView GetImageView(int index) { return *m_ImageViews[index]; }
        u32 ImageCount() { return (u32)m_Images.size(); }
        vk::Format GetImageFormat() { return m_ImageFormat; }
        vk::Extent2D GetExtent() { return m_Extent; }
        u32 Width() { return m_Extent.width; }
        u32 Height() { return m_Extent.height; }

        float ExtentAspectRatio() { return static_cast<float>(m_Extent.width) / static_cast<float>(m_Extent.height); }
        vk::Format FindDepthFormat();

        vk::Result AcquireNextImage(u32& image_index);
        vk::Result SubmitCommandBuffers(const vk::CommandBuffer& buffers, u32& image_index);

        bool CompareFormats(const SwapChain& swapChain) const { return swapChain.m_DepthFormat == m_DepthFormat && swapChain.m_ImageFormat == m_ImageFormat; }

        static i32 GetMaxFramesInFlight() { return MAX_FRAMES_IN_FLIGHT; }

    private:
        static constexpr i32 MAX_FRAMES_IN_FLIGHT = 2;

        void Init();
        void CreateSwapChain();
        void CreateImageViews();
        void CreateRenderPass();
        void CreateDepthResources();
        void CreateFramebuffers();
        void CreateSyncObjects();

        vk::SurfaceFormatKHR ChooseSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& available_formats);
        vk::PresentModeKHR ChoosePresentMode(const std::vector<vk::PresentModeKHR>& available_present_modes);
        vk::Extent2D ChooseExtent(const vk::SurfaceCapabilitiesKHR& capabilities);

        Device& m_Device;
        vk::Extent2D m_WindowExtent;

        vk::UniqueSwapchainKHR m_SwapChain;
        vk::UniqueRenderPass m_RenderPass;

        vk::Format m_ImageFormat;
        vk::Format m_DepthFormat;
        vk::Extent2D m_Extent;

        std::vector<vk::Image> m_Images;
        std::vector<vk::UniqueFramebuffer> m_Framebuffers;
        std::vector<vk::UniqueImageView> m_ImageViews;

        std::vector<vk::UniqueImage> m_DepthImages;
        std::vector<vk::UniqueDeviceMemory> m_DepthImageMemorys;
        std::vector<vk::UniqueImageView> m_DepthImageViews;

        std::vector<vk::UniqueSemaphore> m_ImageAvailableSemaphores;
        std::vector<vk::UniqueSemaphore> m_RenderFinishedSemaphores;
        std::vector<vk::UniqueFence> m_InFlightFences;
        std::vector<vk::Fence> m_ImagesInFlight;

        std::shared_ptr<SwapChain> m_OldSwapChain;

        u32 m_CurrentFrame;
    };
}  // namespace Wraith