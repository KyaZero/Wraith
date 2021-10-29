#pragma once

#include <unordered_map>

#include "Core/Singleton.h"
#include "Texture.h"

namespace Wraith
{
    class TextureManager : public Singleton<TextureManager>
    {
    public:
        const Texture& GetTexture(StringID id);
        const Texture& GetDefaultTexture();

    private:
        void ValidateDefaultTexture();

        Texture m_DefaultTexture;

        std::unordered_map<StringID, Texture> m_TextureMap;
    };
}  // namespace Wraith