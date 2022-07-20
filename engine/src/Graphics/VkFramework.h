#pragma once

#include "Core/Math/Vec4.h"
#include "Texture.h"
#include "VkDevice.h"
#include "VkPipeline.h"
#include "VkSwapchain.h"
#include "VkUtil.h"
#include "Window/Window.h"
#include "TestRenderSystem.h"

namespace Wraith
{
    class VkFramework
    {
    public:
        VkFramework(Window& window);
        ~VkFramework();

        bool Init();

        vk::RenderPass GetSwapChainRenderPass() const { return m_SwapChain->GetRenderPass(); }
        bool IsFrameInProgress() const { return m_IsFrameStarted; }

        vk::CommandBuffer GetCurrentCommandBuffer() const
        {
            ASSERT_LOG(m_IsFrameStarted, "Cannot get command buffer when frame not in progress!");
            return m_CommandBuffers[m_CurrentFrameIndex];
        }

        int GetFrameIndex() const
        {
            ASSERT_LOG(m_IsFrameStarted, "Cannot get frame index when frame not in progress!");
            return m_CurrentFrameIndex;
        }

        vk::CommandBuffer BeginFrame();
        void EndFrame();

        void BeginSwapChainRenderPass(vk::CommandBuffer command_buffer);
        void EndSwapChainRenderPass(vk::CommandBuffer command_buffer);

        static void BeginEvent(std::string name);
        static void EndEvent();

    private:
        void RecreateSwapChain();
        void CreateCommandBuffers();
        void FreeCommandBuffers();

        Window& m_Window;

        std::unique_ptr<Device> m_Device;
        std::unique_ptr<SwapChain> m_SwapChain;
        std::vector<vk::CommandBuffer> m_CommandBuffers;

        TestRenderSystem m_TestSystem;

        u32 m_CurrentImageIndex;
        i32 m_CurrentFrameIndex;
        bool m_IsFrameStarted;
    };
}  // namespace Wraith