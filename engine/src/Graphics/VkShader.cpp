#include "VkShader.h"

#include <shaderc/shaderc.hpp>

Wraith::VkShader::VkShader(vk::UniqueDevice& device, std::underlying_type_t<ShaderType> shader_type, const std::string& shader_path)
{
    std::ifstream file(shader_path);
    if (!file.is_open())
    {
        ERROR_LOG("Failed to open '{}', not compiling shader.", shader_path);
        return;
    }

    auto filename = std::filesystem::path(shader_path).filename();

    std::stringstream buffer;
    buffer << file.rdbuf();
    bool result = Compile(device, shader_type, buffer.str(), filename.string());
    if (!result)
    {
        ERROR_LOG("Failed to compile shader!");
    }
}

bool Wraith::VkShader::Compile(vk::UniqueDevice& device, std::underlying_type_t<ShaderType> shader_type, const std::string& source, const std::string& name)
{
    m_ShaderType = shader_type;

    shaderc::Compiler compiler;
    shaderc::CompileOptions options;
    options.SetOptimizationLevel(shaderc_optimization_level_performance);

    // TODO: Support more shader types.
    const shaderc_shader_kind kind = m_ShaderType == ShaderType::Pixel ? shaderc_fragment_shader : shaderc_vertex_shader;

    shaderc::SpvCompilationResult shader_compilation_result = compiler.CompileGlslToSpv(source, kind, name.c_str(), options);
    if (shader_compilation_result.GetCompilationStatus() != shaderc_compilation_status_success)
    {
        ERROR_LOG("Shader Compilation Error: '{}'", shader_compilation_result.GetErrorMessage());
    }

    std::vector<uint32_t> shader_code = { shader_compilation_result.cbegin(), shader_compilation_result.cend() };
    std::ptrdiff_t shader_size = std::distance(shader_code.begin(), shader_code.end());
    vk::ShaderModuleCreateInfo shader_create_info({}, shader_size * sizeof(uint32_t), shader_code.data());
    m_ShaderModule = HandleResult(device->createShaderModuleUnique(shader_create_info));
    return true;
}

vk::PipelineShaderStageCreateInfo Wraith::VkShader::GetShaderStageInfo()
{
    vk::ShaderStageFlagBits flags = m_ShaderType == ShaderType::Pixel ? vk::ShaderStageFlagBits::eFragment : vk::ShaderStageFlagBits::eVertex;
    return vk::PipelineShaderStageCreateInfo{ {}, flags, *m_ShaderModule, "main" };
}
