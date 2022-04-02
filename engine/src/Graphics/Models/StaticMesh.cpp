#include "StaticMesh.h"

#include "Graphics/Framework.h"

namespace Wraith
{
    StaticMesh::StaticMesh() { }

    StaticMesh::StaticMesh(std::vector<Vertex> vertices, std::vector<u32> indices, Material material)
    {
        SetMaterial(material);
        InitBuffers(vertices, indices);
    }

    StaticMesh::~StaticMesh() { }

    void StaticMesh::InitBuffers(std::vector<Vertex> vertices, std::vector<u32> indices)
    {
        m_VertexBuffer.Init((u32)vertices.size() * sizeof(Vertex), BufferUsage::Immutable, BufferType::Vertex, sizeof(Vertex), vertices.data());
        m_VertexCount = (u32)vertices.size();

        m_IndexBuffer.Init((u32)indices.size() * sizeof(u32), BufferUsage::Immutable, BufferType::Index, sizeof(u32), indices.data());
        m_IndexCount = (u32)indices.size();
    }

    void StaticMesh::SetMaterial(const Material& material) { m_Material = material; }

    Material& StaticMesh::GetMaterial() { return m_Material; }

    Buffer& StaticMesh::GetVertexBuffer() { return m_VertexBuffer; }

    Buffer& StaticMesh::GetIndexBuffer() { return m_IndexBuffer; }

    u32 StaticMesh::GetNumIndices() const { return m_IndexCount; }

    bool StaticMesh::IsValid() const { return m_Material.IsValid(); }

    void StaticMesh::Bind()
    {
        // Framework::GetContext().IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)m_PrimitiveTopology);

        m_VertexBuffer.Bind();
        m_IndexBuffer.Bind();
        m_Material.Bind();
    }

    void StaticMesh::Unbind() { m_Material.Unbind(); }

}  // namespace Wraith