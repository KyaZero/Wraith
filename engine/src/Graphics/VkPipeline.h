#pragma once

#include "VkUtil.h"

using ShaderStages = std::vector<vk::PipelineShaderStageCreateInfo>;

namespace Wraith
{
    struct PipelineConfig
    {
        vk::PipelineViewportStateCreateInfo viewport_state;
        vk::PipelineInputAssemblyStateCreateInfo input_assembly;
        vk::PipelineRasterizationStateCreateInfo rasterization;
        vk::PipelineMultisampleStateCreateInfo multisample;

        vk::PipelineColorBlendAttachmentState color_blend_attachment;
        vk::PipelineColorBlendStateCreateInfo color_blend;
        vk::PipelineDepthStencilStateCreateInfo depth_stencil;

        std::vector<vk::DynamicState> dynamic_states;
        vk::PipelineDynamicStateCreateInfo dynamic_state;

        vk::Viewport viewport;
        vk::Rect2D scissor;

        /* Must be set! */
        vk::PipelineLayout pipeline_layout = nullptr;
        vk::RenderPass render_pass = nullptr;

        uint32_t subpass = 0;
    };

    class Pipeline
    {
    public:
        Pipeline();

        static PipelineConfig GetDefaultConfig();

        void Create(PipelineConfig& config, ShaderStages shader_stages, vk::UniqueDevice& device);

        vk::UniquePipeline& GetPipeline() { return m_Pipeline; }

    private:
        vk::UniquePipelineLayout m_PipelineLayout;
        vk::UniquePipeline m_Pipeline;
    };
}  // namespace Wraith