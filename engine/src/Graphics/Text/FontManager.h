#pragma once

#include <dubu_rect_pack/dubu_rect_pack.hpp>

#include "Core/StringID.h"
#include "Graphics/Text/Font.h"
#include "Graphics/Texture.h"

namespace fw
{
    class FontManager
    {
    public:
        constexpr static u32 ATLAS_SIZE = 1024;

    public:
        struct GlyphData
        {
            Vec2f uv_offset;
            Vec2f uv_scale;
            Vec2f offset;
        };

    public:
        FontManager();
        ~FontManager();

        bool Init();

        std::optional<GlyphData> GetGlyph(StringID font_id, u32 glyph_index);

        Font::DisplayData ShapeText(StringID font_id, std::string_view text, i32 direction);

        f32 GetLineHeight(StringID font_id);

        const Texture& GetAtlas() const
        {
            return m_Atlas;
        }

    private:
        Font* GetFont(StringID font_id);
        bool LoadFont(StringID font_id);

        std::optional<GlyphData> LoadGlyph(Font* font, u32 glyph_index);

        msdfgen::FreetypeHandle* m_FreetypeHandle;
        std::unique_ptr<dubu::rect_pack::Packer> m_Packer;
        std::unordered_map<std::pair<StringID, u32>, std::optional<GlyphData>> m_Glyphs;
        std::unordered_map<StringID, Font> m_Fonts;
        std::unique_ptr<Font> m_DefaultFont;
        Texture m_Atlas;
    };
}  // namespace fw
