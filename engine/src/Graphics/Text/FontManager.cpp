#include "FontManager.h"

#include "Graphics/ContentManager.h"
#include "Graphics/DXUtil.h"

namespace fw
{
    const static auto FONT_CACHE_DIR = TEMP_DIRECTORY / "font_cache";

    FontManager::FontManager()
        : m_Packer(ATLAS_SIZE, ATLAS_SIZE)
    { }

    FontManager::~FontManager()
    {
        for (auto& [_, font] : m_Fonts)
        {
            font.Release();
        }
        if (m_FreetypeHandle)
            msdfgen::deinitializeFreetype(m_FreetypeHandle);
    }

    bool FontManager::Init()
    {
        m_FreetypeHandle = msdfgen::initializeFreetype();
        if (!m_FreetypeHandle)
            return false;

        m_Atlas.Create(TextureCreateInfo{
            .size = { ATLAS_SIZE, ATLAS_SIZE },
            .format = ImageFormat::R32G32B32A32_FLOAT,
            .render_target = false,
            .cpu_access = D3D11_CPU_ACCESS_WRITE,
        });
        SetDebugObjectName(m_Atlas.GetTexture(), "FontAtlas");

        return true;
    }

    std::optional<FontManager::GlyphData> FontManager::GetGlyph(StringID font_id, u16 c)
    {
        if (auto it = m_Glyphs.find({ font_id, c }); it != m_Glyphs.end())
        {
            return it->second;
        }

        if (auto it = m_Fonts.find(font_id); it == m_Fonts.end())
        {
            if (!LoadFont(font_id))
            {
                return std::nullopt;
            }
        }

        return m_Glyphs.emplace(std::make_pair(font_id, c), LoadGlyph(font_id, c)).first->second;
    }

    bool FontManager::LoadFont(StringID font_id)
    {
        auto path = ContentManager::Get()->GetPath(font_id);

        if (!path || !std::filesystem::exists(*path))
            return false;

        auto [it, inserted] = m_Fonts.emplace(font_id, m_FreetypeHandle);
        if (!it->second.Init(*path))
        {
            m_Fonts.erase(it);
            return false;
        }

        return true;
    }

    std::optional<FontManager::GlyphData> FontManager::LoadGlyph(StringID font_id, u16 c)
    {
        auto& font = m_Fonts[font_id];
        auto shape = font.LoadShape(c);
        if (!shape)
            return std::nullopt;

        // 1px padding in atlas to avoid bleeding from other glyphs
        const auto rect = m_Packer.Pack({ shape->width + 1, shape->height + 1 });
        if (!rect)
            return std::nullopt;

        const auto glyph = font.GenerateGlyph(*shape);
        if (!glyph)
            return std::nullopt;

        m_Atlas.Blit(glyph->bitmap.data(), rect->x, rect->y, glyph->width, glyph->height, glyph->stride);

        return GlyphData{
            .uv_offset = { static_cast<f32>(rect->x) / ATLAS_SIZE, static_cast<f32>(rect->y) / ATLAS_SIZE },
            .uv_scale = { static_cast<f32>(glyph->width) / ATLAS_SIZE, static_cast<f32>(glyph->height) / ATLAS_SIZE },
        };
    }
}  // namespace fw