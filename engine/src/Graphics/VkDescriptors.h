#pragma once
#include "VkDevice.h"

namespace Wraith
{
    class DescriptorSetLayout
    {
    public:
        DescriptorSetLayout(Device& device, const std::unordered_map<u32, vk::DescriptorSetLayoutBinding>& bindings);
        ~DescriptorSetLayout() = default;
        DescriptorSetLayout(const DescriptorSetLayout&) = delete;
        DescriptorSetLayout& operator=(const DescriptorSetLayout&) = delete;
        DescriptorSetLayout(DescriptorSetLayout&&) = delete;
        DescriptorSetLayout& operator=(DescriptorSetLayout&&) = delete;

        vk::DescriptorSetLayout GetLayout() const { return *m_DescriptorSetLayout; }

        class Builder
        {
        public:
            Builder(Device& device)
                : m_Device(device) { }

            Builder& AddBinding(u32 binding, vk::DescriptorType descriptor_type, vk::ShaderStageFlags shader_stage_flags, u32 count = 1);
            std::unique_ptr<DescriptorSetLayout> Build() const;

        private:
            Device& m_Device;
            std::unordered_map<u32, vk::DescriptorSetLayoutBinding> m_Bindings{};
        };

    private:
        Device& m_Device;
        vk::UniqueDescriptorSetLayout m_DescriptorSetLayout;
        std::unordered_map<u32, vk::DescriptorSetLayoutBinding> m_Bindings{};
        
        friend class DescriptorWriter;
    };

    class DescriptorPool
    {
    public:
        DescriptorPool(
            Device& device,
            uint32_t max_sets,
            vk::DescriptorPoolCreateFlags pool_flags,
            const std::vector<vk::DescriptorPoolSize>& pool_sizes);
        ~DescriptorPool() = default;
        DescriptorPool(const DescriptorPool&) = delete;
        DescriptorPool& operator=(const DescriptorPool&) = delete;

        bool AllocateDescriptor(const vk::DescriptorSetLayout descriptor_set_layout, vk::DescriptorSet& descriptor_set);
        void FreeDescriptors(std::vector<vk::DescriptorSet>& descriptor_sets);

        void ResetPool();
        
        class Builder
        {
        public:
            Builder(Device& device)
                : m_Device(device) {}

            Builder& AddPoolSize(const vk::DescriptorType descriptor_type, const u32 count);
            Builder& SetPoolFlags(const vk::DescriptorPoolCreateFlags flags);
            Builder& SetMaxSets(const u32 count);

            std::unique_ptr<DescriptorPool> Build() const;

        private:
            Device& m_Device;
            std::vector<vk::DescriptorPoolSize> m_PoolSizes{};
            u32 m_MaxSets = 1000;
            vk::DescriptorPoolCreateFlags m_PoolCreateFlags;
        };

    private:
        Device& m_Device;
        vk::UniqueDescriptorPool m_DescriptorPool;
        
        friend class DescriptorWriter;
    };

    class DescriptorWriter
    {
    public:
        DescriptorWriter(DescriptorSetLayout& set_layout, DescriptorPool& pool);

        DescriptorWriter& WriteBuffer(const u32 binding, vk::DescriptorBufferInfo& buffer_info);
        DescriptorWriter& WriteImage(const u32 binding, vk::DescriptorImageInfo& image_info);

        bool Build(vk::DescriptorSet& set);
        void Overwrite(const vk::DescriptorSet& set);
        
    private:
        DescriptorSetLayout& m_DescriptorSetLayout;
        DescriptorPool& m_DescriptorPool;
        std::vector<vk::WriteDescriptorSet> m_Writes;
    };
}
