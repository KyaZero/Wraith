#include "ModelManager.h"

#include "Graphics/TextureManager.h"

namespace Wraith
{
    Model& ModelManager::GetCube()
    {
        static Model cube_model;

        // No need to re-create the model
        if (cube_model.IsValid())
            return cube_model;

        const Vec4f tangent = { 0.0f, 0.0f, 0.0f, 0.0f };
        const Vec4f bitangent = { 0.0f, 0.0f, 0.0f, 0.0f };
        const Vec4f color = { 1.0f, 1.0f, 1.0f, 1.0f };

        /*
            Vec4f position;
            Vec4f normal;
            Vec4f tangent;
            Vec4f bitangent;
            Vec4f color;
            Vec2f uv;
        */

        std::vector<Vertex> vertices;
        // clang-format off
        vertices.push_back({ Vec4f(-0.5f, 0.5f, -0.5f, 1.0f),  Vec4f(0.0f, 1.0f, 0.0f, 0.0f),  tangent, bitangent, color, Vec2f(1.0f, 0.0f) });
        vertices.push_back({ Vec4f(0.5f, 0.5f, -0.5f, 1.0f),   Vec4f(0.0f, 1.0f, 0.0f, 0.0f),  tangent, bitangent, color, Vec2f(0.0f, 0.0f) });
        vertices.push_back({ Vec4f(0.5f, 0.5f,  0.5f, 1.0f),   Vec4f(0.0f, 1.0f, 0.0f, 0.0f),  tangent, bitangent, color, Vec2f(0.0f, 1.0f) });
        vertices.push_back({ Vec4f(-0.5f, 0.5f,  0.5f, 1.0f),  Vec4f(0.0f, 1.0f, 0.0f, 0.0f),  tangent, bitangent, color, Vec2f(1.0f, 1.0f) });
                                                                                               
        vertices.push_back({ Vec4f(-0.5f, -0.5f,  0.5f, 1.0f), Vec4f(0.0f, -1.0f, 0.0f, 0.0f), tangent, bitangent, color, Vec2f(1.0f, 0.0f) });
        vertices.push_back({ Vec4f(0.5f, -0.5f,  0.5f, 1.0f),  Vec4f(0.0f, -1.0f, 0.0f, 0.0f), tangent, bitangent, color, Vec2f(0.0f, 0.0f) });
        vertices.push_back({ Vec4f(0.5f, -0.5f, -0.5f, 1.0f),  Vec4f(0.0f, -1.0f, 0.0f, 0.0f), tangent, bitangent, color, Vec2f(0.0f, 1.0f) });
        vertices.push_back({ Vec4f(-0.5f, -0.5f, -0.5f, 1.0f), Vec4f(0.0f, -1.0f, 0.0f, 0.0f), tangent, bitangent, color, Vec2f(1.0f, 1.0f) });
                                                                                               
        vertices.push_back({ Vec4f(0.5f,  0.5f,  0.5f, 1.0f),  Vec4f(1.0f, 0.0f, 0.0f, 0.0f),  tangent, bitangent, color, Vec2f(1.0f, 0.0f) });
        vertices.push_back({ Vec4f(0.5f,  0.5f, -0.5f, 1.0f),  Vec4f(1.0f, 0.0f, 0.0f, 0.0f),  tangent, bitangent, color, Vec2f(0.0f, 0.0f) });
        vertices.push_back({ Vec4f(0.5f, -0.5f, -0.5f, 1.0f),  Vec4f(1.0f, 0.0f, 0.0f, 0.0f),  tangent, bitangent, color, Vec2f(0.0f, 1.0f) });
        vertices.push_back({ Vec4f(0.5f, -0.5f,  0.5f, 1.0f),  Vec4f(1.0f, 0.0f, 0.0f, 0.0f),  tangent, bitangent, color, Vec2f(1.0f, 1.0f) });
                                                                                               
        vertices.push_back({ Vec4f(-0.5f,  0.5f, -0.5f, 1.0f), Vec4f(-1.0f, 0.0f, 0.0f, 0.0f), tangent, bitangent, color, Vec2f(1.0f, 0.0f) });
        vertices.push_back({ Vec4f(-0.5f,  0.5f,  0.5f, 1.0f), Vec4f(-1.0f, 0.0f, 0.0f, 0.0f), tangent, bitangent, color, Vec2f(0.0f, 0.0f) });
        vertices.push_back({ Vec4f(-0.5f, -0.5f,  0.5f, 1.0f), Vec4f(-1.0f, 0.0f, 0.0f, 0.0f), tangent, bitangent, color, Vec2f(0.0f, 1.0f) });
        vertices.push_back({ Vec4f(-0.5f, -0.5f, -0.5f, 1.0f), Vec4f(-1.0f, 0.0f, 0.0f, 0.0f), tangent, bitangent, color, Vec2f(1.0f, 1.0f) });
                                                                                               
        vertices.push_back({ Vec4f(-0.5f,  0.5f, 0.5f, 1.0f),  Vec4f(0.0f, 0.0f, 1.0f, 0.0f),  tangent, bitangent, color, Vec2f(1.0f, 0.0f) });
        vertices.push_back({ Vec4f(0.5f,  0.5f, 0.5f, 1.0f),   Vec4f(0.0f, 0.0f, 1.0f, 0.0f),  tangent, bitangent, color, Vec2f(0.0f, 0.0f) });
        vertices.push_back({ Vec4f(0.5f, -0.5f, 0.5f, 1.0f),   Vec4f(0.0f, 0.0f, 1.0f, 0.0f),  tangent, bitangent, color, Vec2f(0.0f, 1.0f) });
        vertices.push_back({ Vec4f(-0.5f, -0.5f, 0.5f, 1.0f),  Vec4f(0.0f, 0.0f, 1.0f, 0.0f),  tangent, bitangent, color, Vec2f(1.0f, 1.0f) });
                                                                                               
        vertices.push_back({ Vec4f(0.5f,  0.5f, -0.5f, 1.0f),  Vec4f(0.0f, 0.0f, -1.0f, 0.0f), tangent, bitangent, color, Vec2f(1.0f, 0.0f) });
        vertices.push_back({ Vec4f(-0.5f,  0.5f, -0.5f, 1.0f), Vec4f(0.0f, 0.0f, -1.0f, 0.0f), tangent, bitangent, color, Vec2f(0.0f, 0.0f) });
        vertices.push_back({ Vec4f(-0.5f, -0.5f, -0.5f, 1.0f), Vec4f(0.0f, 0.0f, -1.0f, 0.0f), tangent, bitangent, color, Vec2f(0.0f, 1.0f) });
        vertices.push_back({ Vec4f(0.5f, -0.5f, -0.5f, 1.0f),  Vec4f(0.0f, 0.0f, -1.0f, 0.0f), tangent, bitangent, color, Vec2f(1.0f, 1.0f) });
        // clang-format on

        std::vector<u32> indices = { 23, 22, 20, 22, 21, 20, 19, 18, 16, 18, 17, 16, 15, 14, 12, 14, 13, 12,
                                     11, 10, 8,  10, 9,  8,  7,  6,  4,  6,  5,  4,  3,  2,  0,  2,  1,  0 };

        Material cube_material(Shader(Shader::Vertex | Shader::Pixel, "assets/engine/shaders/cube.hlsl"));
        cube_material.AddTexture(0, &TextureManager::Get()->GetDefaultTexture());
        cube_model.AddMesh(std::make_shared<StaticMesh>(vertices, indices, cube_material));
        return cube_model;
    }
}  // namespace Wraith