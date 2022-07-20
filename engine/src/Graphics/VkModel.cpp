#include "VkModel.h"

#include "tiny_obj_loader.h"

namespace std
{
    template <>
    struct hash<Wraith::Model::Vertex>
    {
        size_t operator()(Wraith::Model::Vertex const& vertex) const
        {
            size_t seed = 0;
            HashCombine(seed, vertex.position, vertex.color, vertex.normal, vertex.uv);
            return seed;
        }
    };
}  // namespace std

std::vector<vk::VertexInputBindingDescription> Wraith::Model::Vertex::GetBindingDescription()
{
    std::vector<vk::VertexInputBindingDescription> binding_descriptions(1);
    binding_descriptions[0].binding = 0;
    binding_descriptions[0].stride = sizeof(Vertex);
    binding_descriptions[0].inputRate = vk::VertexInputRate::eVertex;
    return binding_descriptions;
}

std::vector<vk::VertexInputAttributeDescription> Wraith::Model::Vertex::GetAttributeDescription()
{
    std::vector<vk::VertexInputAttributeDescription> attribute_descriptions;

    attribute_descriptions.push_back({ 0, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, position) });
    attribute_descriptions.push_back({ 1, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, color) });
    attribute_descriptions.push_back({ 2, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, normal) });
    attribute_descriptions.push_back({ 3, 0, vk::Format::eR32G32Sfloat, offsetof(Vertex, uv) });

    return attribute_descriptions;
}

Wraith::Model::Model(Device& device)
    : m_Device(device)
    , m_HasIndexBuffer(false)
    , m_IndexCount(0)
    , m_VertexCount(0)
{ }

Wraith::Model::Model(Device& device, const std::vector<Vertex>& vertices, const std::vector<u32>& indices)
    : m_Device(device)
    , m_HasIndexBuffer(false)
    , m_IndexCount(0)
    , m_VertexCount(0)
{
    CreateVertexBuffers(vertices);
    CreateIndexBuffers(indices);
}

void Wraith::Model::Create(const std::vector<Vertex>& vertices, const std::vector<u32>& indices)
{
    CreateVertexBuffers(vertices);
    CreateIndexBuffers(indices);
}

void Wraith::Model::Bind(vk::CommandBuffer cmd)
{
    cmd.bindVertexBuffers(0, m_VertexBuffer->GetBuffer(), { 0 });

    if (m_HasIndexBuffer)
    {
        cmd.bindIndexBuffer(m_IndexBuffer->GetBuffer(), 0, vk::IndexType::eUint32);
    }
}

void Wraith::Model::Draw(vk::CommandBuffer cmd)
{
    if (m_HasIndexBuffer)
    {
        cmd.drawIndexed(m_IndexCount, 1, 0, 0, 0);
    }
    else
    {
        cmd.draw(m_VertexCount, 1, 0, 0);
    }
}

void Wraith::Model::LoadFromFile(const std::string& filepath)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str()))
    {
        ERROR_LOG("{}", warn + err);
    }

    std::vector<Vertex> vertices{};
    std::vector<u32> indices{};

    std::unordered_map<Vertex, uint32_t> unique_vertices{};
    for (const auto& shape : shapes)
    {
        for (const auto& index : shape.mesh.indices)
        {
            Vertex vertex{};

            if (index.vertex_index >= 0)
            {
                vertex.position = {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2],
                };

                vertex.color = {
                    attrib.colors[3 * index.vertex_index + 0],
                    attrib.colors[3 * index.vertex_index + 1],
                    attrib.colors[3 * index.vertex_index + 2],
                };
            }

            if (index.normal_index >= 0)
            {
                vertex.normal = {
                    attrib.normals[3 * index.normal_index + 0],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2],
                };
            }

            if (index.texcoord_index >= 0)
            {
                vertex.uv = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    attrib.texcoords[2 * index.texcoord_index + 1],
                };
            }

            if (unique_vertices.count(vertex) == 0)
            {
                unique_vertices[vertex] = static_cast<uint32_t>(vertices.size());
                vertices.push_back(vertex);
            }
            indices.push_back(unique_vertices[vertex]);
        }
    }

    CreateVertexBuffers(vertices);
    CreateIndexBuffers(indices);
}

void Wraith::Model::CreateVertexBuffers(const std::vector<Vertex>& vertices)
{
    m_VertexCount = (u32)vertices.size();
    ASSERT_LOG(m_VertexCount >= 3, "Vertex count must be at least 3!");

    vk::DeviceSize buffer_size = sizeof(vertices[0]) * m_VertexCount;
    u32 vertex_size = sizeof(vertices[0]);

    Buffer staging_buffer(m_Device,
                          vertex_size,
                          m_VertexCount,
                          vk::BufferUsageFlagBits::eTransferSrc,
                          vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);

    staging_buffer.Map();
    staging_buffer.SetData((void*)vertices.data());

    m_VertexBuffer = std::make_unique<Buffer>(m_Device,
                                              vertex_size,
                                              m_VertexCount,
                                              vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst,
                                              vk::MemoryPropertyFlagBits::eDeviceLocal);

    m_Device.CopyBuffer(staging_buffer.GetBuffer(), m_VertexBuffer->GetBuffer(), buffer_size);
}

void Wraith::Model::CreateIndexBuffers(const std::vector<u32>& indices)
{
    m_IndexCount = (u32)indices.size();
    m_HasIndexBuffer = m_IndexCount > 0;

    if (!m_HasIndexBuffer)
    {
        return;
    }

    vk::DeviceSize buffer_size = sizeof(indices[0]) * m_IndexCount;
    u32 index_size = sizeof(indices[0]);

    Buffer staging_buffer(m_Device,
                          index_size,
                          m_IndexCount,
                          vk::BufferUsageFlagBits::eTransferSrc,
                          vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);

    staging_buffer.Map();
    staging_buffer.SetData((void*)indices.data());

    m_IndexBuffer = std::make_unique<Buffer>(m_Device,
                                             index_size,
                                             m_IndexCount,
                                             vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eTransferDst,
                                             vk::MemoryPropertyFlagBits::eDeviceLocal);

    m_Device.CopyBuffer(staging_buffer.GetBuffer(), m_IndexBuffer->GetBuffer(), buffer_size);
}