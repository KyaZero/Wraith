#pragma once

#include "VkDevice.h"

namespace Wraith
{
    class Buffer
    {
    public:
        Buffer(Device& device,
               vk::DeviceSize instance_size,
               u32 instance_count,
               vk::BufferUsageFlags usage_flags,
               vk::MemoryPropertyFlags memory_property_flags,
               vk::DeviceSize min_offset_alignment = 1);
        ~Buffer();

        Buffer(const Buffer&) = delete;
        Buffer& operator=(const Buffer&) = delete;

        vk::Result Map(vk::DeviceSize size = VK_WHOLE_SIZE, vk::DeviceSize offset = 0);
        void Unmap();

        vk::DescriptorBufferInfo DescriptorInfo(vk::DeviceSize size = VK_WHOLE_SIZE, vk::DeviceSize offset = 0);
        void SetData(void* data, vk::DeviceSize size = VK_WHOLE_SIZE, vk::DeviceSize offset = 0) const;
        vk::Result Flush(vk::DeviceSize size = VK_WHOLE_SIZE, vk::DeviceSize offset = 0);

        vk::Buffer GetBuffer() const { return *m_Buffer; }
        void* GetMappedMemory() const { return m_Mapped; }
        u32 GetInstanceCount() const { return m_InstanceCount; }
        vk::DeviceSize GetInstanceSize() const { return m_InstanceSize; }
        vk::DeviceSize GetAlignmentSize() const { return m_AlignmentSize; }
        vk::BufferUsageFlags GetUsageFlags() const { return m_UsageFlags; }
        vk::MemoryPropertyFlags GetMemoryPropertyFlags() const { return m_MemoryPropertyFlags; }
        vk::DeviceSize GetBufferSize() const { return m_BufferSize; }

    private:
        static vk::DeviceSize GetAlignment(vk::DeviceSize instance_size, vk::DeviceSize min_offset_alignment);

        Device& m_Device;
        void* m_Mapped;
        vk::UniqueBuffer m_Buffer;
        vk::UniqueDeviceMemory m_Memory;

        vk::DeviceSize m_BufferSize;
        u32 m_InstanceCount;
        vk::DeviceSize m_InstanceSize;
        vk::DeviceSize m_AlignmentSize;
        vk::BufferUsageFlags m_UsageFlags;
        vk::MemoryPropertyFlags m_MemoryPropertyFlags;
    };
}  // namespace Wraith