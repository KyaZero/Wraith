#include "Core/Types.h"
#include "VkUtil.h"

namespace Wraith
{
    struct FrameInfo
    {
        i32 frame_index;
        f32 frame_time;
        vk::CommandBuffer command_buffer;
        // vk::DescriptorSet globalDescriptorSet;
    };

}  // namespace Wraith