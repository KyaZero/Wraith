#include "VkPipeline.h"

Wraith::Pipeline::Pipeline() { }

Wraith::PipelineConfig Wraith::Pipeline::GetDefaultConfig()
{
    PipelineConfig config;
    config.input_assembly = vk::PipelineInputAssemblyStateCreateInfo({}, vk::PrimitiveTopology::eTriangleList, false);
    // Viewport & Scissor is bound later on, as part of the dynamicstates
    config.viewport_state = vk::PipelineViewportStateCreateInfo({}, 1, nullptr, 1, nullptr);
    config.rasterization =
        vk::PipelineRasterizationStateCreateInfo({}, false, false, vk::PolygonMode::eFill, {}, vk::FrontFace::eClockwise, {}, {}, {}, {}, 1.0f);
    config.multisample = vk::PipelineMultisampleStateCreateInfo({}, vk::SampleCountFlagBits::e1, false, 1.0);
    config.color_blend_attachment = vk::PipelineColorBlendAttachmentState(
        {},
        vk::BlendFactor::eOne,
        vk::BlendFactor::eZero,
        vk::BlendOp::eAdd,
        vk::BlendFactor::eOne,
        vk::BlendFactor::eZero,
        vk::BlendOp::eAdd,
        vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA);

    config.color_blend = vk::PipelineColorBlendStateCreateInfo({}, false, vk::LogicOp::eCopy, 1, nullptr);
    config.depth_stencil = vk::PipelineDepthStencilStateCreateInfo({}, false, false, vk::CompareOp::eLess, false, false, {}, {}, 0.0f, 1.0f);

    config.dynamic_states = { vk::DynamicState::eViewport, vk::DynamicState::eScissor };
    config.dynamic_state = vk::PipelineDynamicStateCreateInfo({}, config.dynamic_states);
    return config;
}

void Wraith::Pipeline::Create(PipelineConfig& config, ShaderStages shader_stages, vk::UniqueDevice& device)
{
    const vk::PipelineVertexInputStateCreateInfo vertex_input_info({}, config.binding_descriptions, config.attribute_descriptions);

    // To avoid the pointer being invalid, it's set to nullptr earlier, and here we set it
    config.color_blend.pAttachments = &config.color_blend_attachment;

    const vk::GraphicsPipelineCreateInfo pipeline_create_info({},
                                                              shader_stages,
                                                              &vertex_input_info,
                                                              &config.input_assembly,
                                                              nullptr,
                                                              &config.viewport_state,
                                                              &config.rasterization,
                                                              &config.multisample,
                                                              &config.depth_stencil,
                                                              &config.color_blend,
                                                              &config.dynamic_state,
                                                              config.pipeline_layout,
                                                              config.render_pass,
                                                              config.subpass);

    m_Pipeline = HandleResult(device->createGraphicsPipelineUnique({}, pipeline_create_info));
}

void Wraith::Pipeline::Bind(vk::CommandBuffer command_buffer)
{
    command_buffer.bindPipeline(vk::PipelineBindPoint::eGraphics, *GetPipeline());
}