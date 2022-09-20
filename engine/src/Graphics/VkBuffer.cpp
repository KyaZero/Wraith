#include "VkBuffer.h"

Wraith::Buffer::Buffer(Device& device,
                       vk::DeviceSize instance_size,
                       u32 instance_count,
                       vk::BufferUsageFlags usage_flags,
                       vk::MemoryPropertyFlags memory_property_flags,
                       vk::DeviceSize min_offset_alignment)
    : m_Device(device)
    , m_Mapped(nullptr)
    , m_InstanceCount(instance_count)
    , m_InstanceSize(instance_size)
    , m_UsageFlags(usage_flags)
    , m_MemoryPropertyFlags(memory_property_flags)
{
    m_AlignmentSize = GetAlignment(instance_size, min_offset_alignment);
    m_BufferSize = m_AlignmentSize * m_InstanceCount;
    m_Device.CreateBuffer(m_BufferSize, m_UsageFlags, m_MemoryPropertyFlags, m_Buffer, m_Memory);
}

Wraith::Buffer::~Buffer()
{
    Unmap();
}

vk::Result Wraith::Buffer::Map(vk::DeviceSize size, vk::DeviceSize offset)
{
    ASSERT_LOG(m_Buffer && m_Memory, "Called Map on buffer before creation!");
    const auto res_value = m_Device.GetDevice()->mapMemory(*m_Memory, offset, size);
    m_Mapped = res_value.value;
    return res_value.result;
}

void Wraith::Buffer::Unmap()
{
    if (m_Mapped)
    {
        m_Device.GetDevice()->unmapMemory(*m_Memory);
        m_Mapped = nullptr;
    }
}
vk::DescriptorBufferInfo Wraith::Buffer::DescriptorInfo(vk::DeviceSize size, vk::DeviceSize offset)
{
    return { *m_Buffer, offset, size };
}

void Wraith::Buffer::SetData(void* data, vk::DeviceSize size, vk::DeviceSize offset) const
{
    ASSERT_LOG(m_Mapped != nullptr, "Cannot copy to unmapped buffer");

    if (size == VK_WHOLE_SIZE)
    {
        memcpy(m_Mapped, data, m_BufferSize);
    }
    else
    {
        c8* mem_offset = (c8*)m_Mapped;
        mem_offset += offset;
        memcpy(mem_offset, data, size);
    }
}
vk::Result Wraith::Buffer::Flush(vk::DeviceSize size, vk::DeviceSize offset)
{
    const vk::MappedMemoryRange mapped_range(*m_Memory, offset, size);
    return m_Device.GetDevice()->flushMappedMemoryRanges(mapped_range);
}

vk::DeviceSize Wraith::Buffer::GetAlignment(vk::DeviceSize instance_size, vk::DeviceSize min_offset_alignment)
{
    if (min_offset_alignment > 0)
    {
        return (instance_size + min_offset_alignment - 1) & ~(min_offset_alignment - 1);
    }

    return instance_size;
}
