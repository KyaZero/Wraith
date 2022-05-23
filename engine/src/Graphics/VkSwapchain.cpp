#include "VkSwapchain.h"

#include "Core/Logger.h"

Wraith::SwapChain::SwapChain(Device& device, vk::Extent2D window_extent)
    : m_Device(device)
    , m_WindowExtent(window_extent)
    , m_CurrentFrame(0)
{
    Init();
}

Wraith::SwapChain::SwapChain(Device& device, vk::Extent2D window_extent, std::shared_ptr<SwapChain> previous)
    : m_Device(device)
    , m_WindowExtent(window_extent)
    , m_CurrentFrame(0)
    , m_OldSwapChain(previous)
{
    Init();
    m_OldSwapChain = nullptr;
}

vk::Format Wraith::SwapChain::FindDepthFormat()
{
    return m_Device.FindSupportedFormat({ vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint },
                                        vk::ImageTiling::eOptimal,
                                        vk::FormatFeatureFlagBits::eDepthStencilAttachment);
}

vk::Result Wraith::SwapChain::AcquireNextImage(u32& image_index)
{
    if (m_Device.GetDevice()->waitForFences(*m_InFlightFences[m_CurrentFrame], true, std::numeric_limits<u64>::max()) != vk::Result::eSuccess)
    {
        ASSERT_LOG(false);
    }

    // auto res_value = m_Device.GetDevice()->acquireNextImageKHR(*m_SwapChain, std::numeric_limits<u64>::max(), *m_ImageAvailableSemaphores[m_CurrentFrame]);
    // image_index = res_value.value;

    // we need to use regular vk functions here, as Vulkan.hpp will assert for certain VkResults which aren't actually erronous.
    auto result = vkAcquireNextImageKHR(
        *m_Device.GetDevice(), *m_SwapChain, std::numeric_limits<u64>::max(), *m_ImageAvailableSemaphores[m_CurrentFrame], VK_NULL_HANDLE, &image_index);

    return vk::Result(result);
}

vk::Result Wraith::SwapChain::SubmitCommandBuffers(const vk::CommandBuffer& buffers, u32& image_index)
{
    if (m_ImagesInFlight[image_index])
    {
        if (m_Device.GetDevice()->waitForFences(m_ImagesInFlight[image_index], true, std::numeric_limits<u64>::max()) != vk::Result::eSuccess)
        {
            ASSERT_LOG(false);
        }
    }

    m_ImagesInFlight[image_index] = *m_InFlightFences[m_CurrentFrame];

    vk::PipelineStageFlags wait_stages = vk::PipelineStageFlagBits::eColorAttachmentOutput;

    vk::SubmitInfo submit_info(*m_ImageAvailableSemaphores[m_CurrentFrame], wait_stages, buffers, *m_RenderFinishedSemaphores[m_CurrentFrame]);

    m_Device.GetDevice()->resetFences(*m_InFlightFences[m_CurrentFrame]);
    vk::Result result = m_Device.GraphicsQueue().submit(submit_info, *m_InFlightFences[m_CurrentFrame]);

    vk::PresentInfoKHR present_info(*m_RenderFinishedSemaphores[m_CurrentFrame], *m_SwapChain, image_index);
    // we need to use regular vk functions here, as Vulkan.hpp will assert for certain VkResults which aren't actually erronous.
    result = (vk::Result)vkQueuePresentKHR(m_Device.PresentQueue(), reinterpret_cast<const VkPresentInfoKHR*>(&present_info));
    m_CurrentFrame = (m_CurrentFrame + 1) & MAX_FRAMES_IN_FLIGHT;

    return result;
}

void Wraith::SwapChain::Init()
{
    CreateSwapChain();
    CreateImageViews();
    CreateRenderPass();
    CreateDepthResources();
    CreateFramebuffers();
    CreateSyncObjects();
}

void Wraith::SwapChain::CreateSwapChain()
{
    auto support = m_Device.GetSwapChainSupport();

    vk::SurfaceFormatKHR surface_format = ChooseSurfaceFormat(support.formats);
    vk::PresentModeKHR present_mode = ChoosePresentMode(support.present_modes);
    vk::Extent2D extent = ChooseExtent(support.capabilities);

    u32 image_count = support.capabilities.minImageCount + 1;
    if (support.capabilities.maxImageCount > 0 && image_count > support.capabilities.maxImageCount)
    {
        image_count = support.capabilities.maxImageCount;
    }

    auto indices = m_Device.FindPhysicalQueueFamilies();
    u32 queue_family_indices[] = { indices.graphics_family, indices.present_family };

    vk::SwapchainCreateInfoKHR create_info({},
                                           *m_Device.Surface(),
                                           image_count,
                                           surface_format.format,
                                           surface_format.colorSpace,
                                           extent,
                                           1,
                                           vk::ImageUsageFlagBits::eColorAttachment,
                                           vk::SharingMode::eExclusive,
                                           0,
                                           nullptr,
                                           support.capabilities.currentTransform,
                                           vk::CompositeAlphaFlagBitsKHR::eOpaque,
                                           present_mode,
                                           true,
                                           m_OldSwapChain == nullptr ? VK_NULL_HANDLE : *m_OldSwapChain->m_SwapChain);

    if (indices.graphics_family != indices.present_family)
    {
        create_info.imageSharingMode = vk::SharingMode::eConcurrent;
        create_info.queueFamilyIndexCount = 2;
        create_info.pQueueFamilyIndices = queue_family_indices;
    }

    m_SwapChain = HandleResult(m_Device.GetDevice()->createSwapchainKHRUnique(create_info));
    m_Images = HandleResult(m_Device.GetDevice()->getSwapchainImagesKHR(*m_SwapChain));
    m_ImageFormat = surface_format.format;
    m_Extent = extent;
}

void Wraith::SwapChain::CreateImageViews()
{
    m_ImageViews.resize(m_Images.size());
    for (u32 i = 0; i < (u32)m_Images.size(); ++i)
    {
        vk::ImageViewCreateInfo view_info(
            {},
            m_Images[i],
            vk::ImageViewType::e2D,
            m_ImageFormat,
            vk::ComponentMapping{ vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA },
            vk::ImageSubresourceRange{ vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1 });

        m_ImageViews[i] = HandleResult(m_Device.GetDevice()->createImageViewUnique(view_info));
    }
}

void Wraith::SwapChain::CreateRenderPass()
{
    vk::AttachmentDescription depth_attachment({},
                                               FindDepthFormat(),
                                               vk::SampleCountFlagBits::e1,
                                               vk::AttachmentLoadOp::eClear,
                                               vk::AttachmentStoreOp::eDontCare,
                                               vk::AttachmentLoadOp::eDontCare,
                                               vk::AttachmentStoreOp::eDontCare,
                                               vk::ImageLayout::eUndefined,
                                               vk::ImageLayout::eDepthStencilAttachmentOptimal);

    vk::AttachmentReference depth_ref(1, vk::ImageLayout::eDepthStencilAttachmentOptimal);

    vk::AttachmentDescription color_attachment({},
                                               GetImageFormat(),
                                               vk::SampleCountFlagBits::e1,
                                               vk::AttachmentLoadOp::eClear,
                                               vk::AttachmentStoreOp::eStore,
                                               vk::AttachmentLoadOp::eDontCare,
                                               vk::AttachmentStoreOp::eDontCare,
                                               vk::ImageLayout::eUndefined,
                                               vk::ImageLayout::ePresentSrcKHR);

    vk::AttachmentReference color_ref(0, vk::ImageLayout::eColorAttachmentOptimal);

    vk::SubpassDescription subpass({}, vk::PipelineBindPoint::eGraphics, {}, color_ref, {}, &depth_ref);

    vk::SubpassDependency dependency(VK_SUBPASS_EXTERNAL,
                                     0,
                                     vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests,
                                     vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests,
                                     {},
                                     vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentWrite);

    std::array<vk::AttachmentDescription, 2> attachments = { color_attachment, depth_attachment };

    vk::RenderPassCreateInfo render_pass_info({}, attachments, subpass, dependency);
    m_RenderPass = HandleResult(m_Device.GetDevice()->createRenderPassUnique(render_pass_info));
}

void Wraith::SwapChain::CreateDepthResources()
{
    vk::Format depth_format = FindDepthFormat();
    m_DepthFormat = depth_format;

    vk::Extent2D extent = GetExtent();

    m_DepthImages.resize(ImageCount());
    m_DepthImageMemorys.resize(ImageCount());
    m_DepthImageViews.resize(ImageCount());

    for (i32 i = 0; i < m_DepthImages.size(); ++i)
    {
        vk::ImageCreateInfo image_info({},
                                       vk::ImageType::e2D,
                                       depth_format,
                                       vk::Extent3D{ extent.width, extent.height, 1 },
                                       1,
                                       1,
                                       vk::SampleCountFlagBits::e1,
                                       vk::ImageTiling::eOptimal,
                                       vk::ImageUsageFlagBits::eDepthStencilAttachment,
                                       vk::SharingMode::eExclusive,
                                       {},
                                       vk::ImageLayout::eUndefined);

        m_Device.CreateImageWithInfo(image_info, vk::MemoryPropertyFlagBits::eDeviceLocal, m_DepthImages[i], m_DepthImageMemorys[i]);

        vk::ImageViewCreateInfo view_info(
            {},
            *m_DepthImages[i],
            vk::ImageViewType::e2D,
            depth_format,
            vk::ComponentMapping{ vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA },
            vk::ImageSubresourceRange{ vk::ImageAspectFlagBits::eDepth, 0, 1, 0, 1 });

        m_DepthImageViews[i] = HandleResult(m_Device.GetDevice()->createImageViewUnique(view_info));
    }
}

void Wraith::SwapChain::CreateFramebuffers()
{
    m_Framebuffers.resize(ImageCount());
    for (u32 i = 0; i < ImageCount(); ++i)
    {
        std::array<vk::ImageView, 2> attachments = { *m_ImageViews[i], *m_DepthImageViews[i] };

        vk::Extent2D extent = GetExtent();
        vk::FramebufferCreateInfo framebuffer_info({}, *m_RenderPass, attachments, extent.width, extent.height, 1);

        m_Framebuffers[i] = HandleResult(m_Device.GetDevice()->createFramebufferUnique(framebuffer_info));
    }
}

void Wraith::SwapChain::CreateSyncObjects()
{
    m_ImageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    m_RenderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    m_InFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
    m_ImagesInFlight.resize(ImageCount(), VK_NULL_HANDLE);

    vk::SemaphoreCreateInfo semaphore_info;

    vk::FenceCreateInfo fence_info(vk::FenceCreateFlagBits::eSignaled);
    for (u32 i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
    {
        m_ImageAvailableSemaphores[i] = HandleResult(m_Device.GetDevice()->createSemaphoreUnique(semaphore_info));
        m_RenderFinishedSemaphores[i] = HandleResult(m_Device.GetDevice()->createSemaphoreUnique(semaphore_info));
        m_InFlightFences[i] = HandleResult(m_Device.GetDevice()->createFenceUnique(fence_info));
    }
}

vk::SurfaceFormatKHR Wraith::SwapChain::ChooseSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& available_formats)
{
    for (const auto& format : available_formats)
    {
        if (format.format == vk::Format::eB8G8R8A8Srgb && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
        {
            return format;
        }
    }

    return available_formats[0];
}

vk::PresentModeKHR Wraith::SwapChain::ChoosePresentMode(const std::vector<vk::PresentModeKHR>& available_present_modes)
{
    // TODO: Make a config for swapchain, where you can select a desired present mode at creation
    // and if you change it later, it recreates the swapchain with desired present mode
    // e.g m_SwapChain.SetPresentMode(PresentMode::Immediate);

    for (const auto& mode : available_present_modes)
    {
        if (mode == vk::PresentModeKHR::eMailbox)
        {
            INFO_LOG("Present Mode: Mailbox");
            return mode;
        }
    }

    for (const auto& mode : available_present_modes)
    {
        if (mode == vk::PresentModeKHR::eImmediate)
        {
            INFO_LOG("Present Mode: Immediate");
            return mode;
        }
    }

    INFO_LOG("Present Mode: V-Sync")
    return vk::PresentModeKHR::eFifo;
}

vk::Extent2D Wraith::SwapChain::ChooseExtent(const vk::SurfaceCapabilitiesKHR& capabilities)
{
    if (capabilities.currentExtent.width != std::numeric_limits<u32>::max())
    {
        return capabilities.currentExtent;
    }
    else
    {
        vk::Extent2D actual_extent = m_WindowExtent;
        actual_extent.width = std::clamp(actual_extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actual_extent.height = std::clamp(actual_extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actual_extent;
    }
}
