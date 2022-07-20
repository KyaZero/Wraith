#include "TestRenderSystem.h"

Wraith::TestRenderSystem::TestRenderSystem(Device& device)
    : RenderSystem("Test System", device)
    , m_Model(device)
{ }

bool Wraith::TestRenderSystem::Init(vk::RenderPass render_pass)
{
    // Shitty triangle
    std::vector<Model::Vertex> vertices;
    vertices.push_back(Model::Vertex{ .position = { -0.5f, 0.5f, 0 }, .color = { 0.0f, 0.0f, 1.0f }, .normal = {}, .uv = {} });
    vertices.push_back(Model::Vertex{ .position = { 0.5f, 0.5f, 0 }, .color = { 0.0f, 1.0f, 0.0f }, .normal = {}, .uv = {} });
    vertices.push_back(Model::Vertex{ .position = { 0.0f, -0.5f, 0 }, .color = { 1.0f, 0.0f, 0.0f }, .normal = {}, .uv = {} });
    m_Model.Create(vertices, { 0, 1, 2 });

    //m_Model.LoadFromFile("assets/teapot.obj");

    return RenderSystem::Init(render_pass);
}

void Wraith::TestRenderSystem::Render(FrameInfo& frame_info)
{
    vk::CommandBuffer& cmd = frame_info.command_buffer;
    m_Pipeline->Bind(cmd);

    m_Model.Bind(cmd);
    m_Model.Draw(cmd);
}

void Wraith::TestRenderSystem::CreatePipelineLayout()
{
    m_PipelineLayout = HandleResult(m_Device.GetDevice()->createPipelineLayoutUnique({}));
}

void Wraith::TestRenderSystem::CreatePipeline(vk::RenderPass render_pass)
{
    VkShader vertex_shader(m_Device.GetDevice(), VkShader::ShaderType::Vertex, "assets/engine/shaders/triangle.vert");
    VkShader fragment_shader(m_Device.GetDevice(), VkShader::ShaderType::Pixel, "assets/engine/shaders/triangle.frag");

    PipelineConfig config = Pipeline::GetDefaultConfig();
    config.render_pass = render_pass;
    config.pipeline_layout = *m_PipelineLayout;
    config.attribute_descriptions = Model::Vertex::GetAttributeDescription();
    config.binding_descriptions = Model::Vertex::GetBindingDescription();

    m_Pipeline->Create(config, { vertex_shader.GetShaderStageInfo(), fragment_shader.GetShaderStageInfo() }, m_Device.GetDevice());
}