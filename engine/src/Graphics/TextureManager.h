#pragma once

#include <unordered_map>

#include "Core/Singleton.h"
#include "Texture.h"
#include "TextureID.h"

namespace fw
{
    class TextureManager : public Singleton<TextureManager>
    {
    public:
        const Texture& GetTexture(TextureID id);
        const Texture& GetDefaultTexture();

    private:
        void ValidateDefaultTexture();

        Texture m_DefaultTexture;

        std::unordered_map<TextureID, Texture> m_TextureMap;
    };
}  // namespace fw