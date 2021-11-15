#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Texture.h"

namespace Wraith
{
    class Material
    {
    public:
        Material() = default;
        Material(const Shader& shader);
        Material(Shader&& shader);
        ~Material() = default;

        void AddTexture(u32 slot, const Texture& texture);
        void SetShader(const Shader& shader);
        void SetShader(Shader&& shader);

        void Bind();
        void Unbind();

    private:
        Shader m_Shader;

        struct BindableTexture
        {
            u32 slot;
            Texture texture;
        };

        std::vector<BindableTexture> m_TextureList;
    };
}  // namespace Wraith