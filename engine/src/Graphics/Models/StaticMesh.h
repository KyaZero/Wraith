#pragma once

#include "Core/Math/Vec.h"
#include "Graphics/Buffer.h"
#include "Graphics/Shader.h"
#include "Graphics/Texture.h"
#include "Material.h"

namespace Wraith
{
    enum class PrimitiveTopology : u8
    {
        PointList = 1,
        LineList = 2,
        LineStrip = 3,
        TriangleList = 4,
        TriangleStrip = 5,
    };

    struct Vertex
    {
        Vec4f position;
        Vec4f normal;
        Vec4f tangent;
        Vec4f bitangent;
        Vec4f color;
        Vec2f uv;
    };

    class StaticMesh
    {
    public:
        StaticMesh();
        StaticMesh(std::vector<Vertex> vertices, std::vector<u32> indices, Material material);
        ~StaticMesh();

        void InitBuffers(std::vector<Vertex> vertices, std::vector<u32> indices);
        void SetMaterial(const Material& material);

        Material& GetMaterial();
        Buffer& GetVertexBuffer();
        Buffer& GetIndexBuffer();

        u32 GetNumIndices() const;

        bool IsValid() const;

        void Bind();
        void Unbind();

    private:
        Buffer m_IndexBuffer;
        Buffer m_VertexBuffer;
        Material m_Material;
        u32 m_VertexCount;
        u32 m_IndexCount;
        PrimitiveTopology m_PrimitiveTopology = PrimitiveTopology::TriangleList;
    };
}  // namespace Wraith