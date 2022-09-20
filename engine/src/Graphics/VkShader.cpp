#include "VkShader.h"

#include <shaderc/shaderc.hpp>

Wraith::VkShader::VkShader(vk::UniqueDevice& device, std::underlying_type_t<ShaderType> shader_type, const std::string& shader_path)
{
    m_ShaderType = shader_type;

    if (LoadSpvIfExists(device, shader_path))
        return;

    const std::ifstream file(shader_path);
    if (!file.is_open())
    {
        ERROR_LOG("Failed to open '{}', not compiling shader.", shader_path);
        return;
    }

    const auto filename = std::filesystem::path(shader_path).filename();

    std::stringstream buffer;
    buffer << file.rdbuf();
    const bool result = Compile(device, shader_type, buffer.str(), filename.string());
    if (!result)
    {
        ERROR_LOG("Failed to compile shader!");
    }
}

bool Wraith::VkShader::Compile(vk::UniqueDevice& device, std::underlying_type_t<ShaderType> shader_type, const std::string& source, const std::string& name)
{
    const shaderc::Compiler compiler;
    shaderc::CompileOptions options;
    options.SetOptimizationLevel(shaderc_optimization_level_performance);

    // TODO: Support more shader types.
    const shaderc_shader_kind kind = m_ShaderType == ShaderType::Pixel ? shaderc_fragment_shader : shaderc_vertex_shader;

    const shaderc::SpvCompilationResult shader_compilation_result = compiler.CompileGlslToSpv(source, kind, name.c_str(), options);
    if (shader_compilation_result.GetCompilationStatus() != shaderc_compilation_status_success)
    {
        ERROR_LOG("Shader Compilation Error: '{}'", shader_compilation_result.GetErrorMessage());
    }

    std::vector<uint32_t> shader_code = { shader_compilation_result.cbegin(), shader_compilation_result.cend() };
    const std::ptrdiff_t shader_size = std::distance(shader_code.begin(), shader_code.end());
    const vk::ShaderModuleCreateInfo shader_create_info({}, shader_size * sizeof(uint32_t), shader_code.data());
    m_ShaderModule = HandleResult(device->createShaderModuleUnique(shader_create_info));
    return true;
}

vk::PipelineShaderStageCreateInfo Wraith::VkShader::GetShaderStageInfo()
{
    const vk::ShaderStageFlagBits flags = m_ShaderType == ShaderType::Pixel ? vk::ShaderStageFlagBits::eFragment : vk::ShaderStageFlagBits::eVertex;
    return vk::PipelineShaderStageCreateInfo{ {}, flags, *m_ShaderModule, "main" };
}

bool Wraith::VkShader::IsFileTimestampNewer(const std::string& a, const std::string& b) const
{
    const auto a_timestamp = std::filesystem::last_write_time(a);
    const auto b_timestamp = std::filesystem::last_write_time(b);

    if (a_timestamp >= b_timestamp)
        return true;

    return false;
}

bool Wraith::VkShader::LoadSpvIfExists(vk::UniqueDevice& device, const std::string& shader_path)
{
    std::string spv_path = shader_path + ".spv";

    if (!std::filesystem::exists(spv_path))
        return false;

    if (!IsFileTimestampNewer(spv_path, shader_path))
        return false;

    std::ifstream file{ spv_path, std::ios::ate | std::ios::binary };

    if (!file.is_open())
    {
        ERROR_LOG("failed to open file: {}", spv_path);
        return false;
    }

    const size_t shader_size = static_cast<size_t>(file.tellg());
    std::vector<char> shader_code(shader_size);

    file.seekg(0);
    file.read(shader_code.data(), shader_size);

    file.close();

    const vk::ShaderModuleCreateInfo shader_create_info({}, shader_size, (u32*)shader_code.data());
    m_ShaderModule = HandleResult(device->createShaderModuleUnique(shader_create_info));

    return true;
}
