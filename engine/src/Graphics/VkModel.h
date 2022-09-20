#pragma once

#include "Core/Math/Vec.h"
#include "VkBuffer.h"
#include "VkDevice.h"

//#define GLM_FORCE_RADIANS
//#define GLM_FORCE_DEPTH_ZERO_TO_ONE
//#include <glm/glm.hpp>

#include <vector>

namespace Wraith
{
    class Model
    {
    public:
        struct Vertex
        {
            Vec3f position;
            Vec3f color;
            Vec3f normal;
            Vec2f uv;

            static std::vector<vk::VertexInputBindingDescription> GetBindingDescription();
            static std::vector<vk::VertexInputAttributeDescription> GetAttributeDescription();

            bool operator==(const Vertex& other) const
            {
                return position == other.position && color == other.color && normal == other.normal && uv == other.uv;
            }
        };

        Model(Device& device);
        Model(Device& device, const std::vector<Vertex>& vertices, const std::vector<u32>& indices);
        ~Model() = default;

        void Create(const std::vector<Vertex>& vertices, const std::vector<u32>& indices);

        void Bind(vk::CommandBuffer cmd) const;
        void Draw(vk::CommandBuffer cmd) const;
        void LoadFromFile(const std::string& filepath);

    private:
        void CreateVertexBuffers(const std::vector<Vertex>& vertices);
        void CreateIndexBuffers(const std::vector<u32>& indices);

        Device& m_Device;
        std::unique_ptr<Buffer> m_VertexBuffer;
        u32 m_VertexCount;

        bool m_HasIndexBuffer;
        std::unique_ptr<Buffer> m_IndexBuffer;
        u32 m_IndexCount;
    };

}  // namespace Wraith