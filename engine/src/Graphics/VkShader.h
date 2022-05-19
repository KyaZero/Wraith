#pragma once
#include <Core/Types.h>
#include <string>

#include "VkUtil.h"

namespace Wraith
{
    class VkShader
    {
    public:
        enum ShaderType : u8
        {
            Vertex = 1 << 0,
            Geometry = 1 << 1,
            Pixel = 1 << 2,
            Compute = 1 << 3,
        };

        VkShader() = default;
        VkShader(vk::UniqueDevice& device, std::underlying_type_t<ShaderType> shader_type, const std::string& shader_path);

        bool Compile(vk::UniqueDevice& device, std::underlying_type_t<ShaderType> shader_type, const std::string& source, const std::string& name = "shader");

        vk::PipelineShaderStageCreateInfo GetShaderStageInfo();

    private:
        vk::UniqueShaderModule m_ShaderModule;
        std::underlying_type_t<ShaderType> m_ShaderType;
    };
}  // namespace Wraith