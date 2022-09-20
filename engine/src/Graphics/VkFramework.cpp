#include "VkFramework.h"

#include <set>

#include "VkShader.h"

namespace Wraith
{
    VkFramework::VkFramework(Window& window)
        : m_Window(window)
        , m_Device(std::make_unique<Device>(window))
        , m_IsFrameStarted(false)
        , m_CurrentFrameIndex(0)
        , m_CurrentImageIndex(0)
        , m_TestSystem(*m_Device)
    { }

    VkFramework::~VkFramework()
    {
        FreeCommandBuffers();
        Window::UnregisterResizeCallback(this);
    }

    bool VkFramework::Init()
    {
        RecreateSwapChain();
        CreateCommandBuffers();

        m_TestSystem.Init(m_SwapChain->GetRenderPass());

        INFO_LOG("Finished initializing Vulkan Framework!");
        return true;
    }

    vk::CommandBuffer VkFramework::BeginFrame()
    {
        ASSERT_LOG(!m_IsFrameStarted, "Can't call BeginFrame while frame already in progress!");

        const vk::Result result = m_SwapChain->AcquireNextImage(m_CurrentImageIndex);
        if (result == vk::Result::eErrorOutOfDateKHR)
        {
            RecreateSwapChain();
            return {};
        }

        if (result != vk::Result::eSuccess && result != vk::Result::eSuboptimalKHR)
        {
            ASSERT_LOG(false, "Failed to acquire swap chain image!");
        }

        m_IsFrameStarted = true;

        const auto command_buffer = GetCurrentCommandBuffer();

        if (command_buffer.begin(vk::CommandBufferBeginInfo{}) != vk::Result::eSuccess)
        {
            ASSERT_LOG(false, "Failed to begin recording Command Buffer!");
        }

        BeginSwapChainRenderPass(command_buffer);

        FrameInfo info;
        info.command_buffer = command_buffer;
        m_TestSystem.Render(info);

        return command_buffer;
    }

    void VkFramework::EndFrame()
    {
        ASSERT_LOG(m_IsFrameStarted, "Can't call EndFrame while frame is not in progress!");

        const auto command_buffer = GetCurrentCommandBuffer();

        EndSwapChainRenderPass(command_buffer);

        if (command_buffer.end() != vk::Result::eSuccess)
        {
            ASSERT_LOG(false, "Failed to record Command Buffer!");
        }

        const vk::Result result = m_SwapChain->SubmitCommandBuffers(command_buffer, m_CurrentImageIndex);
        if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR || m_Window.WasResized())
        {
            m_Window.ResetResizedFlag();
            RecreateSwapChain();
        }
        else if (result != vk::Result::eSuccess)
        {
            ASSERT_LOG(false, "Failed to present swap chain image!");
        }

        m_IsFrameStarted = false;
        m_CurrentFrameIndex = (m_CurrentFrameIndex + 1) % SwapChain::GetMaxFramesInFlight();
    }

    void VkFramework::BeginSwapChainRenderPass(vk::CommandBuffer command_buffer) const
    {
        ASSERT_LOG(m_IsFrameStarted, "Can't call BeginSwapChainRenderPass if frame is not in progress!");
        ASSERT_LOG(command_buffer == GetCurrentCommandBuffer() && "Can't begin render pass on command buffer from a different frame");

        std::array<vk::ClearValue, 2> clear_values = {};
        clear_values[0].color = std::array<f32, 4>{ 0.01f, 0.01f, 0.01f, 1.0f };
        clear_values[1].depthStencil = vk::ClearDepthStencilValue({ 1.0f, 1 });

        const vk::RenderPassBeginInfo render_pass_info(
            m_SwapChain->GetRenderPass(), m_SwapChain->GetFrameBuffer(m_CurrentImageIndex), vk::Rect2D({ 0, 0 }, m_SwapChain->GetExtent()), clear_values);

        command_buffer.beginRenderPass(render_pass_info, vk::SubpassContents::eInline);

        vk::Viewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<f32>(m_SwapChain->GetExtent().width);
        viewport.height = static_cast<f32>(m_SwapChain->GetExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        const vk::Rect2D scissor{ { 0, 0 }, m_SwapChain->GetExtent() };
        command_buffer.setViewport(0, 1, &viewport);
        command_buffer.setScissor(0, 1, &scissor);
    }

    void VkFramework::EndSwapChainRenderPass(vk::CommandBuffer command_buffer) const
    {
        ASSERT_LOG(m_IsFrameStarted, "Can't call EndSwapChainRenderPass if frame is not in progress!");
        ASSERT_LOG(command_buffer == GetCurrentCommandBuffer() && "Can't end render pass on command buffer from a different frame");

        command_buffer.endRenderPass();
    }

    void VkFramework::BeginEvent(std::string name) { }

    void VkFramework::EndEvent() { }

    void VkFramework::RecreateSwapChain()
    {
        auto extent = vk::Extent2D(m_Window.GetSize().x, m_Window.GetSize().y);
        while (extent.width == 0 || extent.height == 0)
        {
            extent = vk::Extent2D(m_Window.GetSize().x, m_Window.GetSize().y);
            glfwWaitEvents();
        }

        vkDeviceWaitIdle(*m_Device->GetDevice());

        if (m_SwapChain == nullptr)
        {
            m_SwapChain = std::make_unique<SwapChain>(*m_Device, extent);
        }
        else
        {
            std::shared_ptr<SwapChain> old_swap_chain = std::move(m_SwapChain);

            m_SwapChain = std::make_unique<SwapChain>(*m_Device, extent, old_swap_chain);

            if (!old_swap_chain->CompareFormats(*m_SwapChain))
            {
                ASSERT_LOG(false, "Swap chain image(or depth) format has changed!");
            }
        }
    }

    void VkFramework::CreateCommandBuffers()
    {
        m_CommandBuffers.resize(SwapChain::GetMaxFramesInFlight());

        const vk::CommandBufferAllocateInfo alloc_info(*m_Device->GetCommandPool(), vk::CommandBufferLevel::ePrimary, (u32)m_CommandBuffers.size());
        m_CommandBuffers = HandleResult(m_Device->GetDevice()->allocateCommandBuffers(alloc_info));
    }

    void VkFramework::FreeCommandBuffers() { m_Device->GetDevice()->freeCommandBuffers(*m_Device->GetCommandPool(), m_CommandBuffers); }

}  // namespace Wraith