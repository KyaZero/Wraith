#include "TextureManager.h"

#include "Core/Filewatcher.h"

namespace Wraith
{
    const Texture& TextureManager::GetTexture(TextureID id)
    {
        auto it = m_TextureMap.find(id);
        if (it == m_TextureMap.end())
        {
            auto path = ContentManager::Get()->GetPath(id);

            if (!path)
            {
                return GetDefaultTexture();
            }

            if (path->extension() != ".png" && path->extension() != ".jpg")
            {
                WARNING_LOG("Unsupported Texture Format '{}'", path->string());
                return GetDefaultTexture();
            }

            const auto texture_loader = [=]() {
                Texture new_texture;
                if (!new_texture.LoadFromFile(path->string()))
                    return false;
                m_TextureMap[id] = new_texture;
                return true;
            };

            if (!texture_loader())
            {
                ERROR_LOG("Failed to load texture from file: {}", path->string());
                return GetDefaultTexture();
            }

            Filewatcher::Get()->Watch(path->string(), [=]() { texture_loader(); });
            it = m_TextureMap.find(id);
        }

        return it->second;
    }

    const Texture& TextureManager::GetDefaultTexture()
    {
        ValidateDefaultTexture();
        return m_DefaultTexture;
    }

    void TextureManager::ValidateDefaultTexture()
    {
        if (m_DefaultTexture.IsValid())
            return;

        m_DefaultTexture.LoadFromFile("assets/textures/default.png");
    }
}  // namespace Wraith