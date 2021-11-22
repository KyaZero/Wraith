#pragma once

#include "Graphics/Shader.h"
#include "Graphics/Texture.h"

namespace Wraith
{
    class Material
    {
    private:
        struct BindableTexture
        {
            u32 slot;
            const Texture* texture;
        };

    public:
        Material() = default;
        Material(const Shader& shader);
        Material(Shader&& shader);
        ~Material() = default;

        void AddTexture(u32 slot, const Texture* texture);
        void AddTexture(u32 slot, StringID texture);
        void SetShader(const Shader& shader);
        void SetShader(Shader&& shader);

        void Bind();
        void Unbind();

        const Shader& GetShader() { return m_Shader; }
        std::vector<BindableTexture>& GetTextures() { return m_TextureList; }

    private:
        Shader m_Shader;

        std::vector<BindableTexture> m_TextureList;
    };
}  // namespace Wraith