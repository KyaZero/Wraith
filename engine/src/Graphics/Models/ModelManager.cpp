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
        vertices.push_back({ { 0.5f,  0.5f,  0.5f, 1.0f}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {1,1,1,1}, {1, 0} });
        vertices.push_back({ { 0.5f,  0.5f, -0.5f, 1.0f}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {1,1,1,1}, {1, 0} });
        vertices.push_back({ { 0.5f, -0.5f,  0.5f, 1.0f}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {1,1,1,1}, {1, 1} });
        vertices.push_back({ {-0.5f,  0.5f,  0.5f, 1.0f}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {1,1,1,1}, {0, 0} });
        vertices.push_back({ { 0.5f, -0.5f, -0.5f, 1.0f}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {1,1,1,1}, {1, 1} });
        vertices.push_back({ {-0.5f,  0.5f, -0.5f, 1.0f}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {1,1,1,1}, {0, 0} });
        vertices.push_back({ {-0.5f, -0.5f,  0.5f, 1.0f}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {1,1,1,1}, {0, 1} });
        vertices.push_back({ {-0.5f, -0.5f, -0.5f, 1.0f}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {1,1,1,1}, {0, 1} });

        std::vector<u32> indices =
        {
            0, 2, 1,
            0, 1, 3,
            0, 3, 2,
            1, 2, 4,
            2, 3, 6,
            3, 1, 5,
            4, 5, 1,
            5, 6, 3,
            6, 4, 2,
            7, 6, 5,
            7, 5, 4,
            7, 4, 6
        };

        std::reverse(indices.begin(), indices.end());
        // clang-format on

        Material cube_material(Shader(Shader::Vertex | Shader::Pixel, "assets/engine/shaders/cube.hlsl"));
        cube_material.AddTexture(0, &TextureManager::Get()->GetDefaultTexture());
        cube_model.AddMesh(std::make_shared<StaticMesh>(vertices, indices, cube_material));
        return cube_model;
    }
}  // namespace Wraith