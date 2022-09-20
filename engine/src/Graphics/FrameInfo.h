#pragma once
#include "Core/Types.h"
#include "VkUtil.h"

namespace Wraith
{
    struct GlobalBuffer
    {
        Mat4f projection;
        Mat4f view;
        Vec4f color;
    };
    
    struct FrameInfo
    {
        i32 frame_index{};
        f32 frame_time{};
        vk::CommandBuffer command_buffer;
        vk::DescriptorSet global_descriptor_set;
    };

}  // namespace Wraith