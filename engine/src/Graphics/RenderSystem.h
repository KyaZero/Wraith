#pragma once

#include "Core/Constants.h"
#include "FrameInfo.h"
#include "Framework.h"
#include "RenderCommand.h"
#include "VkDevice.h"
#include "VkPipeline.h"
#include "Window/Window.h"

namespace Wraith
{
    /*
        Basic interface class for a RenderSystem
    */
    template <typename Command>
    class RenderSystem
    {
    public:
        RenderSystem(const char* name, Device& device)
            : m_Name(name)
            , m_Device(device)
            , m_Pipeline(std::make_unique<Pipeline>())
        { }

        virtual ~RenderSystem() { }

        virtual void Submit(const Command& command) { m_Commands.push_back(command); }

        virtual bool Init(vk::RenderPass render_pass, vk::DescriptorSetLayout global_set_layout)
        {
            CreatePipelineLayout(global_set_layout);
            CreatePipeline(render_pass);
            return true;
        }

        virtual void Render(FrameInfo& frameInfo) = 0;

    protected:
        virtual void CreatePipelineLayout([[maybe_unused]] vk::DescriptorSetLayout global_set_layout) { }
        virtual void CreatePipeline([[maybe_unused]] vk::RenderPass render_pass) { }

        Device& m_Device;
        std::unique_ptr<Pipeline> m_Pipeline;
        vk::UniquePipelineLayout m_PipelineLayout;

    private:
        std::vector<Command> m_Commands;
        std::string m_Name;
    };
}  // namespace Wraith