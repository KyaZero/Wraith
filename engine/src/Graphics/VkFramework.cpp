#include "VkFramework.h"

#include <set>

#include "VkShader.h"

static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
                                                    VkDebugUtilsMessageTypeFlagsEXT message_type,
                                                    const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
                                                    void* user_data)
{
    if (message_severity & VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
    {
        ERROR_LOG("Validation Layer: {}", callback_data->pMessage);
        return VK_FALSE;
    }
    else if (message_severity & VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
    {
        WARNING_LOG("Validation Layer: {}", callback_data->pMessage);
        return VK_FALSE;
    }
    else if (message_severity & VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
    {
        INFO_LOG("Validation Layer: {}", callback_data->pMessage);
        return VK_FALSE;
    }
    else if (message_severity & VkDebugUtilsMessageSeverityFlagBitsEXT::VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)
    {
        VERBOSE_LOG("Validation Layer: {}", callback_data->pMessage);
        return VK_FALSE;
    }

    return VK_TRUE;
}

namespace Wraith
{
    VkFramework::VkFramework(Window& window)
        : m_Window(window)
    { }

    VkFramework::~VkFramework() { Window::UnregisterResizeCallback(this); }

    bool VkFramework::Init()
    {
        vk::ApplicationInfo app_info("Wraith", VK_MAKE_VERSION(1, 0, 0), "Wraith Engine", VK_MAKE_VERSION(1, 0, 0), VK_API_VERSION_1_2);

        uint32_t glfw_extension_count = 0;
        const char** glfw_extensions;
        glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);
        std::vector<const char*> required_extensions(glfw_extensions, glfw_extensions + glfw_extension_count);
        required_extensions.push_back("VK_EXT_debug_utils");

        std::vector<const char*> layers = { "VK_LAYER_KHRONOS_validation" };

        vk::InstanceCreateInfo instance_info({}, &app_info, 0, nullptr, (u32)required_extensions.size(), required_extensions.data());
        m_Instance = HandleResult(vk::createInstanceUnique(instance_info));

        vk::DispatchLoaderDynamic dispatch_loader(*m_Instance, vkGetInstanceProcAddr);

        auto severity_flags = vk::DebugUtilsMessageSeverityFlagBitsEXT::eError | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
                              vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo;

        auto type_flags = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
                          vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance;

        vk::DebugUtilsMessengerCreateInfoEXT messenger_create_info({}, severity_flags, type_flags, DebugCallback);
        auto messenger = HandleResult(m_Instance->createDebugUtilsMessengerEXTUnique(messenger_create_info, nullptr, dispatch_loader));

        INFO_LOG("Available Vulkan Instance Extensions: ")
        for (const auto& extension : HandleResult(vk::enumerateInstanceExtensionProperties()))
        {
            INFO_LOG("\t {}", extension.extensionName);
        }

        VkSurfaceKHR surface;
        if (glfwCreateWindowSurface(*m_Instance, m_Window.GetHandle(), nullptr, &surface) != VK_SUCCESS)
        {
            return false;
        }

        m_Surface = vk::UniqueSurfaceKHR(surface, *m_Instance);

        std::vector<vk::PhysicalDevice> physical_devices = HandleResult(m_Instance->enumeratePhysicalDevices());

        INFO_LOG("Available Physical Devices: ");
        for (auto& device : physical_devices)
        {
            INFO_LOG("\t {}", device.getProperties().deviceName);
        }

        // Goes for first discrete gpu if possible, we might update this to do a more thorough selection later.
        m_PhysicalDevice = physical_devices[std::distance(
            physical_devices.begin(), std::find_if(physical_devices.begin(), physical_devices.end(), [](const vk::PhysicalDevice& physical_device) {
                return physical_device.getProperties().deviceType == vk::PhysicalDeviceType::eDiscreteGpu;
            }))];

        // Only looks for a graphics queue right now, later on we will want a compute queue, and maybe a transfer queue as well.
        std::vector<vk::QueueFamilyProperties> queue_family_properties = m_PhysicalDevice.getQueueFamilyProperties();
        u64 graphics_queue_family_index =
            std::distance(queue_family_properties.begin(),
                          std::find_if(queue_family_properties.begin(), queue_family_properties.end(), [](vk::QueueFamilyProperties const& qfp) {
                              return qfp.queueFlags & vk::QueueFlagBits::eGraphics;
                          }));

        u64 present_queue_family_index = 0u;
        for (auto i = 0ul; i < queue_family_properties.size(); i++)
        {
            if (HandleResult(m_PhysicalDevice.getSurfaceSupportKHR((u32)i, *m_Surface)))
            {
                present_queue_family_index = i;
            }
        }

        std::set<u32> unique_queue_family_indices = { (u32)graphics_queue_family_index, (u32)present_queue_family_index };

        std::vector<uint32_t> family_indices = { unique_queue_family_indices.begin(), unique_queue_family_indices.end() };

        std::vector<vk::DeviceQueueCreateInfo> queue_create_infos;

        f32 queue_priority = 0.0f;
        for (auto& queueFamilyIndex : unique_queue_family_indices)
        {
            queue_create_infos.push_back(vk::DeviceQueueCreateInfo({}, (u32)queueFamilyIndex, 1, &queue_priority));
        }

        const std::vector<const char*> device_extensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

        vk::DeviceCreateInfo device_info(
            {}, (u32)queue_create_infos.size(), queue_create_infos.data(), 0u, nullptr, (u32)device_extensions.size(), device_extensions.data());

        m_Device = HandleResult(m_PhysicalDevice.createDeviceUnique(device_info));

        u32 image_count = 2;

        struct SM
        {
            vk::SharingMode sharing_mode;
            u32 indices_count;
            u32* indices;
        } sharing_mode_util{ (graphics_queue_family_index != present_queue_family_index) ? SM{ vk::SharingMode::eConcurrent, 2u, family_indices.data() }
                                                                                         : SM{ vk::SharingMode::eExclusive, 0u, nullptr } };

        vk::SurfaceCapabilitiesKHR capabilities = HandleResult(m_PhysicalDevice.getSurfaceCapabilitiesKHR(*m_Surface));
        std::vector<vk::SurfaceFormatKHR> formats = HandleResult(m_PhysicalDevice.getSurfaceFormatsKHR(*m_Surface));

        vk::SurfaceFormatKHR selected_format = vk::Format::eUndefined;
        for (auto& format : formats)
        {
            if (format.format == vk::Format::eB8G8R8A8Unorm)
            {
                selected_format = format;
                break;
            }
        }

        // If there's no supported format that we want, we grab whatever exists.
        if (selected_format.format == vk::Format::eUndefined)
        {
            selected_format = formats[0];
        }

        Vec2u window_size = m_Window.GetSize();
        vk::Extent2D selected_extent = { 0, 0 };
        if (capabilities.minImageExtent <= window_size.x && capabilities.maxImageExtent >= window_size.y)
        {
            selected_extent = vk::Extent2D(window_size.x, window_size.y);
        }
        else
        {
            selected_extent = vk::Extent2D(capabilities.maxImageExtent);
        }

        vk::SwapchainCreateInfoKHR swap_chain_create_info({},
                                                          m_Surface.get(),
                                                          image_count,
                                                          selected_format.format,
                                                          vk::ColorSpaceKHR::eSrgbNonlinear,
                                                          selected_extent,
                                                          1,
                                                          vk::ImageUsageFlagBits::eColorAttachment,
                                                          sharing_mode_util.sharing_mode,
                                                          sharing_mode_util.indices_count,
                                                          sharing_mode_util.indices,
                                                          vk::SurfaceTransformFlagBitsKHR::eIdentity,
                                                          vk::CompositeAlphaFlagBitsKHR::eOpaque,
                                                          vk::PresentModeKHR::eImmediate,
                                                          true,
                                                          nullptr);

        m_SwapChain = HandleResult(m_Device->createSwapchainKHRUnique(swap_chain_create_info));

        std::vector<vk::Image> swap_chain_images = HandleResult(m_Device->getSwapchainImagesKHR(m_SwapChain.get()));
        m_ImageViews.reserve(swap_chain_images.size());

        for (auto image : swap_chain_images)
        {
            vk::ImageViewCreateInfo image_view_create_info(
                vk::ImageViewCreateFlags(),
                image,
                vk::ImageViewType::e2D,
                selected_format.format,
                vk::ComponentMapping{ vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA },
                vk::ImageSubresourceRange{ vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1 });
            m_ImageViews.push_back(HandleResult(m_Device->createImageViewUnique(image_view_create_info)));
        }

        vk::AttachmentDescription color_attachment({},
                                                   selected_format.format,
                                                   vk::SampleCountFlagBits::e1,
                                                   vk::AttachmentLoadOp::eClear,
                                                   vk::AttachmentStoreOp::eStore,
                                                   {},
                                                   {},
                                                   {},
                                                   vk::ImageLayout::ePresentSrcKHR);
        vk::AttachmentReference color_attachment_ref(0, vk::ImageLayout::eColorAttachmentOptimal);
        vk::SubpassDescription subpass({}, vk::PipelineBindPoint::eGraphics, 0, nullptr, 1, &color_attachment_ref);

        vk::SubpassDependency subpass_dependency(VK_SUBPASS_EXTERNAL,
                                                 0,
                                                 vk::PipelineStageFlagBits::eColorAttachmentOutput,
                                                 vk::PipelineStageFlagBits::eColorAttachmentOutput,
                                                 {},
                                                 vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite);

        m_RenderPass = HandleResult(m_Device->createRenderPassUnique(vk::RenderPassCreateInfo({}, 1, &color_attachment, 1, &subpass, 1, &subpass_dependency)));

        vk::SemaphoreCreateInfo semaphore_create_info{};
        m_ImageAvailableSemaphore = HandleResult(m_Device->createSemaphoreUnique(semaphore_create_info));
        m_RenderFinishedSemaphore = HandleResult(m_Device->createSemaphoreUnique(semaphore_create_info));

        VkShader vertex_shader(m_Device, VkShader::ShaderType::Vertex, "assets/engine/shaders/triangle.vert");
        VkShader fragment_shader(m_Device, VkShader::ShaderType::Pixel, "assets/engine/shaders/triangle.frag");

        PipelineConfig config = Pipeline::GetDefaultConfig();
        config.render_pass = *m_RenderPass;
        config.rasterization.setPolygonMode(vk::PolygonMode::eLine);

        m_Pipeline.Create(config, { vertex_shader.GetShaderStageInfo(), fragment_shader.GetShaderStageInfo() }, m_Device);

        m_Framebuffers.resize(image_count);
        for (size_t i = 0; i < m_ImageViews.size(); ++i)
        {
            m_Framebuffers[i] = HandleResult(m_Device->createFramebufferUnique(
                vk::FramebufferCreateInfo({}, *m_RenderPass, 1, &(*m_ImageViews[i]), selected_extent.width, selected_extent.height, 1)));
        }

        m_CommandPool = HandleResult(m_Device->createCommandPoolUnique(vk::CommandPoolCreateInfo({}, (u32)graphics_queue_family_index)));

        m_CommandBuffers = HandleResult(m_Device->allocateCommandBuffersUnique(
            vk::CommandBufferAllocateInfo(*m_CommandPool, vk::CommandBufferLevel::ePrimary, (u32)m_Framebuffers.size())));

        m_GraphicsQueue = m_Device->getQueue((u32)graphics_queue_family_index, 0);
        m_PresentQueue = m_Device->getQueue((u32)graphics_queue_family_index, 0);

        for (size_t i = 0; i < m_CommandBuffers.size(); ++i)
        {
            auto& cmd = m_CommandBuffers[i];
            vk::CommandBufferBeginInfo begin_info{};
            vk::Result result = cmd->begin(begin_info);
            vk::ClearValue clear_values{};
            vk::RenderPassBeginInfo render_pass_begin_info(*m_RenderPass, *m_Framebuffers[i], vk::Rect2D({ 0, 0 }, selected_extent), 1, &clear_values);
            cmd->beginRenderPass(render_pass_begin_info, vk::SubpassContents::eInline);

            vk::Viewport viewport{};
            viewport.x = 0.0f;
            viewport.y = 0.0f;
            viewport.width = static_cast<f32>(window_size.x);
            viewport.height = static_cast<f32>(window_size.y);
            viewport.minDepth = 0.0f;
            viewport.maxDepth = 1.0f;

            vk::Rect2D scissor{ { 0, 0 }, selected_extent };
            cmd->setViewport(0, 1, &viewport);
            cmd->setScissor(0, 1, &scissor);

            cmd->bindPipeline(vk::PipelineBindPoint::eGraphics, *m_Pipeline.GetPipeline());
            cmd->draw(3, 1, 0, 0);
            cmd->endRenderPass();
            result = cmd->end();
        }

        Window::RegisterResizeCallback(this, [&](u32 width, u32 height) { ResizeBackbuffer(width, height); });
        INFO_LOG("Finished initializing Vulkan Framework!");
        return true;
    }

    void VkFramework::BeginFrame(const Vec4f& clear_color) { }

    void VkFramework::EndFrame()
    {
        auto image_index = HandleResult(m_Device->acquireNextImageKHR(*m_SwapChain, std::numeric_limits<u64>::max(), *m_ImageAvailableSemaphore, {}),
                                        { vk::Result::eSuccess, vk::Result::eTimeout, vk::Result::eNotReady, vk::Result::eSuboptimalKHR });
        vk::PipelineStageFlags wait_stage_mask = vk::PipelineStageFlagBits::eColorAttachmentOutput;

        vk::SubmitInfo submit_info(
            1, &m_ImageAvailableSemaphore.get(), &wait_stage_mask, 1, &m_CommandBuffers[image_index].get(), 1, &m_RenderFinishedSemaphore.get());
        vk::Result result = m_GraphicsQueue.submit(submit_info, {});

        vk::PresentInfoKHR present_info(1, &m_RenderFinishedSemaphore.get(), 1, &m_SwapChain.get(), &image_index);
        result = m_PresentQueue.presentKHR(present_info);

        result = m_Device->waitIdle();
    }

    void VkFramework::ResizeBackbuffer(u32 width, u32 height)
    {
        if (width == 0 || height == 0)
            return;

        VERBOSE_LOG("Resized backbuffer to ({}, {})", width, height);
    }

    void VkFramework::BeginEvent(std::string name) { }

    void VkFramework::EndEvent() { }
}  // namespace Wraith