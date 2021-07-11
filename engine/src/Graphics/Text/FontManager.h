#pragma once

#include <dubu_rect_pack/dubu_rect_pack.hpp>

#include "Core/StringID.h"
#include "Graphics/Text/Font.h"
#include "Graphics/Texture.h"

namespace fw
{
    class FontManager
    {
        constexpr static u32 ATLAS_SIZE = 256;

    public:
        struct GlyphData
        {
            Vec2f uv_offset;
            Vec2f uv_scale;
        };

    public:
        FontManager();
        ~FontManager();

        bool Init();

        std::optional<GlyphData> GetGlyph(StringID font_id, u16 c);

        const Texture& GetAtlas() const
        {
            return m_Atlas;
        }

    private:
        bool LoadFont(StringID font_id);

        std::optional<GlyphData> LoadGlyph(StringID font_id, u16 c);

        msdfgen::FreetypeHandle* m_FreetypeHandle;
        dubu::rect_pack::Packer m_Packer;
        std::unordered_map<std::pair<StringID, u16>, std::optional<GlyphData>> m_Glyphs;
        std::unordered_map<StringID, Font> m_Fonts;
        Texture m_Atlas;
    };
}  // namespace fw
