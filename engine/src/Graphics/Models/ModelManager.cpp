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
        vertices.push_back({ Vec4f(-0.5f, 0.5f, -0.5f, 1.0f),  Vec4f(0.0f, 1.0f, 0.0f, 0.0f),  Vec4f(0.0f, 0.0f, 0.0f, 0.0f), Vec4f(0.0f, 0.0f, 0.0f, 0.0f), Vec4f(1.0f, 1.0f, 1.0f, 1.0f), Vec2f(1.0f, 1.0f) });
        vertices.push_back({ Vec4f(0.5f, 0.5f, -0.5f, 1.0f),   Vec4f(0.0f, 1.0f, 0.0f, 0.0f),  Vec4f(0.0f, 0.0f, 0.0f, 0.0f), Vec4f(0.0f, 0.0f, 0.0f, 0.0f), Vec4f(1.0f, 1.0f, 1.0f, 1.0f), Vec2f(0.0f, 1.0f) });
        vertices.push_back({ Vec4f(0.5f, 0.5f,  0.5f, 1.0f),   Vec4f(0.0f, 1.0f, 0.0f, 0.0f),  Vec4f(0.0f, 0.0f, 0.0f, 0.0f), Vec4f(0.0f, 0.0f, 0.0f, 0.0f), Vec4f(1.0f, 1.0f, 1.0f, 1.0f), Vec2f(0.0f, 0.0f) });
        vertices.push_back({ Vec4f(-0.5f, 0.5f,  0.5f, 1.0f),  Vec4f(0.0f, 1.0f, 0.0f, 0.0f),  Vec4f(0.0f, 0.0f, 0.0f, 0.0f), Vec4f(0.0f, 0.0f, 0.0f, 0.0f), Vec4f(1.0f, 1.0f, 1.0f, 1.0f), Vec2f(1.0f, 0.0f) });
                                 
        vertices.push_back({ Vec4f(-0.5f, -0.5f,  0.5f, 1.0f), Vec4f(0.0f, -1.0f, 0.0f, 0.0f), Vec4f(0.0f, 0.0f, 0.0f, 0.0f), Vec4f(0.0f, 0.0f, 0.0f, 0.0f), Vec4f(1.0f, 1.0f, 1.0f, 1.0f), Vec2f(1.0f, 1.0f) });
        vertices.push_back({ Vec4f(0.5f, -0.5f,  0.5f, 1.0f),  Vec4f(0.0f, -1.0f, 0.0f, 0.0f), Vec4f(0.0f, 0.0f, 0.0f, 0.0f), Vec4f(0.0f, 0.0f, 0.0f, 0.0f), Vec4f(1.0f, 1.0f, 1.0f, 1.0f), Vec2f(0.0f, 1.0f) });
        vertices.push_back({ Vec4f(0.5f, -0.5f, -0.5f, 1.0f),  Vec4f(0.0f, -1.0f, 0.0f, 0.0f), Vec4f(0.0f, 0.0f, 0.0f, 0.0f), Vec4f(0.0f, 0.0f, 0.0f, 0.0f), Vec4f(1.0f, 1.0f, 1.0f, 1.0f), Vec2f(0.0f, 0.0f) });
        vertices.push_back({ Vec4f(-0.5f, -0.5f, -0.5f, 1.0f), Vec4f(0.0f, -1.0f, 0.0f, 0.0f), Vec4f(0.0f, 0.0f, 0.0f, 0.0f), Vec4f(0.0f, 0.0f, 0.0f, 0.0f), Vec4f(1.0f, 1.0f, 1.0f, 1.0f), Vec2f(1.0f, 0.0f) });
                                 
        vertices.push_back({ Vec4f(0.5f,  0.5f,  0.5f, 1.0f),  Vec4f(1.0f, 0.0f, 0.0f, 0.0f),  Vec4f(0.0f, 0.0f, 0.0f, 0.0f), Vec4f(0.0f, 0.0f, 0.0f, 0.0f), Vec4f(1.0f, 1.0f, 1.0f, 1.0f), Vec2f(1.0f, 1.0f) });
        vertices.push_back({ Vec4f(0.5f,  0.5f, -0.5f, 1.0f),  Vec4f(1.0f, 0.0f, 0.0f, 0.0f),  Vec4f(0.0f, 0.0f, 0.0f, 0.0f), Vec4f(0.0f, 0.0f, 0.0f, 0.0f), Vec4f(1.0f, 1.0f, 1.0f, 1.0f), Vec2f(0.0f, 1.0f) });
        vertices.push_back({ Vec4f(0.5f, -0.5f, -0.5f, 1.0f),  Vec4f(1.0f, 0.0f, 0.0f, 0.0f),  Vec4f(0.0f, 0.0f, 0.0f, 0.0f), Vec4f(0.0f, 0.0f, 0.0f, 0.0f), Vec4f(1.0f, 1.0f, 1.0f, 1.0f), Vec2f(0.0f, 0.0f) });
        vertices.push_back({ Vec4f(0.5f, -0.5f,  0.5f, 1.0f),  Vec4f(1.0f, 0.0f, 0.0f, 0.0f),  Vec4f(0.0f, 0.0f, 0.0f, 0.0f), Vec4f(0.0f, 0.0f, 0.0f, 0.0f), Vec4f(1.0f, 1.0f, 1.0f, 1.0f), Vec2f(1.0f, 0.0f) });
                                 
        vertices.push_back({ Vec4f(-0.5f,  0.5f, -0.5f, 1.0f), Vec4f(-1.0f, 0.0f, 0.0f, 0.0f), Vec4f(0.0f, 0.0f, 0.0f, 0.0f), Vec4f(0.0f, 0.0f, 0.0f, 0.0f), Vec4f(1.0f, 1.0f, 1.0f, 1.0f), Vec2f(1.0f, 1.0f) });
        vertices.push_back({ Vec4f(-0.5f,  0.5f,  0.5f, 1.0f), Vec4f(-1.0f, 0.0f, 0.0f, 0.0f), Vec4f(0.0f, 0.0f, 0.0f, 0.0f), Vec4f(0.0f, 0.0f, 0.0f, 0.0f), Vec4f(1.0f, 1.0f, 1.0f, 1.0f), Vec2f(0.0f, 1.0f) });
        vertices.push_back({ Vec4f(-0.5f, -0.5f,  0.5f, 1.0f), Vec4f(-1.0f, 0.0f, 0.0f, 0.0f), Vec4f(0.0f, 0.0f, 0.0f, 0.0f), Vec4f(0.0f, 0.0f, 0.0f, 0.0f), Vec4f(1.0f, 1.0f, 1.0f, 1.0f), Vec2f(0.0f, 0.0f) });
        vertices.push_back({ Vec4f(-0.5f, -0.5f, -0.5f, 1.0f), Vec4f(-1.0f, 0.0f, 0.0f, 0.0f), Vec4f(0.0f, 0.0f, 0.0f, 0.0f), Vec4f(0.0f, 0.0f, 0.0f, 0.0f), Vec4f(1.0f, 1.0f, 1.0f, 1.0f), Vec2f(1.0f, 0.0f) });
                                 
        vertices.push_back({ Vec4f(-0.5f,  0.5f, 0.5f, 1.0f),  Vec4f(0.0f, 0.0f, 1.0f, 0.0f),  Vec4f(0.0f, 0.0f, 0.0f, 0.0f), Vec4f(0.0f, 0.0f, 0.0f, 0.0f), Vec4f(1.0f, 1.0f, 1.0f, 1.0f), Vec2f(1.0f, 1.0f) });
        vertices.push_back({ Vec4f(0.5f,  0.5f, 0.5f, 1.0f),   Vec4f(0.0f, 0.0f, 1.0f, 0.0f),  Vec4f(0.0f, 0.0f, 0.0f, 0.0f), Vec4f(0.0f, 0.0f, 0.0f, 0.0f), Vec4f(1.0f, 1.0f, 1.0f, 1.0f), Vec2f(0.0f, 1.0f) });
        vertices.push_back({ Vec4f(0.5f, -0.5f, 0.5f, 1.0f),   Vec4f(0.0f, 0.0f, 1.0f, 0.0f),  Vec4f(0.0f, 0.0f, 0.0f, 0.0f), Vec4f(0.0f, 0.0f, 0.0f, 0.0f), Vec4f(1.0f, 1.0f, 1.0f, 1.0f), Vec2f(0.0f, 0.0f) });
        vertices.push_back({ Vec4f(-0.5f, -0.5f, 0.5f, 1.0f),  Vec4f(0.0f, 0.0f, 1.0f, 0.0f),  Vec4f(0.0f, 0.0f, 0.0f, 0.0f), Vec4f(0.0f, 0.0f, 0.0f, 0.0f), Vec4f(1.0f, 1.0f, 1.0f, 1.0f), Vec2f(1.0f, 0.0f) });
                                 
        vertices.push_back({ Vec4f(0.5f,  0.5f, -0.5f, 1.0f),  Vec4f(0.0f, 0.0f, -1.0f, 0.0f), Vec4f(0.0f, 0.0f, 0.0f, 0.0f), Vec4f(0.0f, 0.0f, 0.0f, 0.0f), Vec4f(1.0f, 1.0f, 1.0f, 1.0f), Vec2f(1.0f, 1.0f) });
        vertices.push_back({ Vec4f(-0.5f,  0.5f, -0.5f, 1.0f), Vec4f(0.0f, 0.0f, -1.0f, 0.0f), Vec4f(0.0f, 0.0f, 0.0f, 0.0f), Vec4f(0.0f, 0.0f, 0.0f, 0.0f), Vec4f(1.0f, 1.0f, 1.0f, 1.0f), Vec2f(0.0f, 1.0f) });
        vertices.push_back({ Vec4f(-0.5f, -0.5f, -0.5f, 1.0f), Vec4f(0.0f, 0.0f, -1.0f, 0.0f), Vec4f(0.0f, 0.0f, 0.0f, 0.0f), Vec4f(0.0f, 0.0f, 0.0f, 0.0f), Vec4f(1.0f, 1.0f, 1.0f, 1.0f), Vec2f(0.0f, 0.0f) });
        vertices.push_back({ Vec4f(0.5f, -0.5f, -0.5f, 1.0f),  Vec4f(0.0f, 0.0f, -1.0f, 0.0f), Vec4f(0.0f, 0.0f, 0.0f, 0.0f), Vec4f(0.0f, 0.0f, 0.0f, 0.0f), Vec4f(1.0f, 1.0f, 1.0f, 1.0f), Vec2f(1.0f, 0.0f) });
        // clang-format on

        std::vector<u32> indices = { 0,  1,  2,  0,  2,  3,  4,  5,  6,  4,  6,  7,  8,  9,  10, 8,  10, 11,
                                     12, 13, 14, 12, 14, 15, 16, 17, 18, 16, 18, 19, 20, 21, 22, 20, 22, 23 };

        Material cube_material(Shader(Shader::Vertex | Shader::Pixel, "assets/engine/shaders/cube.hlsl"));
        cube_material.AddTexture(0, &TextureManager::Get()->GetDefaultTexture());
        cube_model.AddMesh(std::make_shared<StaticMesh>(vertices, indices, cube_material));
        return cube_model;
    }
}  // namespace Wraith