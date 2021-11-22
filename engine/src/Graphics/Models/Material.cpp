#include "Material.h"

#include "Graphics/TextureManager.h"

namespace Wraith
{
    Material::Material(const Shader& shader) { SetShader(shader); }

    Material::Material(Shader&& shader) { SetShader(std::forward<Shader>(shader)); }

    void Material::AddTexture(u32 slot, const Texture* texture) { m_TextureList.push_back({ slot, texture }); }

    void Material::AddTexture(u32 slot, StringID texture)
    {
        m_TextureList.push_back({ slot, &TextureManager::Get()->GetTexture(texture) });
    }

    void Material::SetShader(const Shader& shader) { m_Shader = shader; }

    void Material::SetShader(Shader&& shader) { m_Shader = std::move(shader); }

    void Material::Bind()
    {
        m_Shader.Bind();
        for (auto& [slot, texture] : m_TextureList)
        {
            texture->Bind(slot);
        }
    }

    void Material::Unbind()
    {
        m_Shader.Unbind();
        for (auto& [slot, texture] : m_TextureList)
        {
            texture->Unbind(slot);
        }
    }
}  // namespace Wraith