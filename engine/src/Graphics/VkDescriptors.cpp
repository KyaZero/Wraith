#include "VkDescriptors.h"

Wraith::DescriptorSetLayout::DescriptorSetLayout(Device& device, const std::unordered_map<u32, vk::DescriptorSetLayoutBinding>& bindings)
    : m_Device(device)
    , m_Bindings(bindings)
{
    std::vector<vk::DescriptorSetLayoutBinding> set_layout_bindings{};
    for (auto [key, value] : bindings)
    {
        set_layout_bindings.push_back(value);
    }

    const vk::DescriptorSetLayoutCreateInfo create_info({}, set_layout_bindings);
    m_DescriptorSetLayout = HandleResult(device.GetDevice()->createDescriptorSetLayoutUnique(create_info));
}

Wraith::DescriptorSetLayout::Builder& Wraith::DescriptorSetLayout::Builder::AddBinding(const u32 binding,
                                                                                       const vk::DescriptorType descriptor_type,
                                                                                       const vk::ShaderStageFlags shader_stage_flags,
                                                                                       const u32 count)
{
    ASSERT_LOG(!m_Bindings.contains(binding), "Binding is already in use.");

    const vk::DescriptorSetLayoutBinding layout_binding(binding, descriptor_type, count, shader_stage_flags);
    m_Bindings[binding] = layout_binding;
    return *this;
}

std::unique_ptr<Wraith::DescriptorSetLayout> Wraith::DescriptorSetLayout::Builder::Build() const
{
    return std::make_unique<DescriptorSetLayout>(m_Device, m_Bindings);
}

Wraith::DescriptorPool::DescriptorPool(Device& device,
                                       const uint32_t max_sets,
                                       const vk::DescriptorPoolCreateFlags pool_flags,
                                       const std::vector<vk::DescriptorPoolSize>& pool_sizes)
    : m_Device(device)
{
    const vk::DescriptorPoolCreateInfo create_info(pool_flags, max_sets, pool_sizes);
    m_DescriptorPool = HandleResult(m_Device.GetDevice()->createDescriptorPoolUnique(create_info));
}

bool Wraith::DescriptorPool::AllocateDescriptor(const vk::DescriptorSetLayout descriptor_set_layout, vk::DescriptorSet& descriptor_set)
{
    const vk::DescriptorSetAllocateInfo allocate_info(*m_DescriptorPool, 1, &descriptor_set_layout);
    auto [result, allocated] = m_Device.GetDevice()->allocateDescriptorSets(allocate_info);

    if (result != vk::Result::eSuccess)
    {
        // TODO: Handle allocating more descriptor sets when the pool runs out, perhaps using a DescriptorPoolManager
        // which could track whenever we exceed the limit and then create another pool with the same amount.
        return false;
    }

    descriptor_set = allocated[0];
    return true;
}

void Wraith::DescriptorPool::FreeDescriptors(std::vector<vk::DescriptorSet>& descriptor_sets)
{
    m_Device.GetDevice()->freeDescriptorSets(*m_DescriptorPool, descriptor_sets);
}

void Wraith::DescriptorPool::ResetPool()
{
    m_Device.GetDevice()->resetDescriptorPool(*m_DescriptorPool);
}

Wraith::DescriptorPool::Builder& Wraith::DescriptorPool::Builder::AddPoolSize(const vk::DescriptorType descriptor_type, const u32 count)
{
    m_PoolSizes.emplace_back(descriptor_type, count);
    return *this;
}

Wraith::DescriptorPool::Builder& Wraith::DescriptorPool::Builder::SetPoolFlags(const vk::DescriptorPoolCreateFlags flags)
{
    m_PoolCreateFlags = flags;
    return *this;
}

Wraith::DescriptorPool::Builder& Wraith::DescriptorPool::Builder::SetMaxSets(const u32 count)
{
    m_MaxSets = count;
    return *this;
}

std::unique_ptr<Wraith::DescriptorPool> Wraith::DescriptorPool::Builder::Build() const
{
    return std::make_unique<DescriptorPool>(m_Device, m_MaxSets, m_PoolCreateFlags, m_PoolSizes);
}
Wraith::DescriptorWriter::DescriptorWriter(DescriptorSetLayout& set_layout, DescriptorPool& pool)
    : m_DescriptorSetLayout(set_layout)
    , m_DescriptorPool(pool)
{ }

Wraith::DescriptorWriter& Wraith::DescriptorWriter::WriteBuffer(const u32 binding, vk::DescriptorBufferInfo& buffer_info)
{
    ASSERT_LOG(m_DescriptorSetLayout.m_Bindings.contains(binding), "Layout does not contain specified binding!");

    const vk::DescriptorSetLayoutBinding& binding_description = m_DescriptorSetLayout.m_Bindings[binding];

    ASSERT_LOG(binding_description.descriptorCount == 1, "Binding single descriptor info, but binding expects multiple!");

    vk::WriteDescriptorSet write;
    write.setDstBinding(binding);
    write.setBufferInfo(buffer_info);
    write.setDescriptorType(binding_description.descriptorType);
    write.setDescriptorCount(1);

    m_Writes.push_back(write);
    return *this;
}

Wraith::DescriptorWriter& Wraith::DescriptorWriter::WriteImage(const u32 binding, vk::DescriptorImageInfo& image_info)
{
    ASSERT_LOG(m_DescriptorSetLayout.m_Bindings.contains(binding), "Layout does not contain specified binding!");

    const vk::DescriptorSetLayoutBinding& binding_description = m_DescriptorSetLayout.m_Bindings[binding];

    ASSERT_LOG(binding_description.descriptorCount == 1, "Binding single descriptor info, but binding expects multiple!");

    vk::WriteDescriptorSet write;
    write.setDstBinding(binding);
    write.setImageInfo(image_info);
    write.setDescriptorType(binding_description.descriptorType);
    write.setDescriptorCount(1);

    return *this;
}

bool Wraith::DescriptorWriter::Build(vk::DescriptorSet& set)
{
    if (!m_DescriptorPool.AllocateDescriptor(m_DescriptorSetLayout.GetLayout(), set))
        return false;

    Overwrite(set);
    return true;
}

void Wraith::DescriptorWriter::Overwrite(const vk::DescriptorSet& set)
{
    for (auto& write : m_Writes)
    {
        write.dstSet = set;
    }

    m_DescriptorPool.m_Device.GetDevice()->updateDescriptorSets(m_Writes, {});
}
