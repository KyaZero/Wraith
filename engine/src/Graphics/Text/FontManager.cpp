#include "FontManager.h"

#include "Graphics/ContentManager.h"
#include "Graphics/DXUtil.h"

namespace fw
{
    FontManager::FontManager()
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

        m_DefaultFont = std::make_unique<Font>(m_FreetypeHandle);
        if (!m_DefaultFont->Init("assets/engine/fonts/roboto-regular.ttf"))
            return false;

        m_Atlas.Create(TextureCreateInfo{
            .size = { ATLAS_SIZE, ATLAS_SIZE },
            .format = ImageFormat::R32G32B32A32_FLOAT,
            .render_target = false,
            .cpu_access = D3D11_CPU_ACCESS_WRITE,
        });
        SetDebugObjectName(m_Atlas.GetTexture(), "FontAtlas");

        m_Packer = std::make_unique<dubu::rect_pack::Packer>(ATLAS_SIZE, ATLAS_SIZE);

        return true;
    }

    std::optional<FontManager::GlyphData> FontManager::GetGlyph(StringID font_id, msdfgen::GlyphIndex glyph_index)
    {
        if (auto it = m_Glyphs.find({ font_id, glyph_index.getIndex() }); it != m_Glyphs.end())
        {
            return it->second;
        }

        if (auto font = GetFont(font_id); font)
        {
            const auto [it, inserted] =
                m_Glyphs.emplace(std::make_pair(font_id, glyph_index), LoadGlyph(font, glyph_index));
            return it->second;
        }

        return std::nullopt;
    }

    Font::DisplayData FontManager::ShapeText(StringID font_id, std::string_view text, i32 direction)
    {
        if (auto font = GetFont(font_id); font)
            return font->ShapeText(text, direction);
        return { {}, {} };
    }

    f32 FontManager::GetLineHeight(StringID font_id)
    {
        if (auto font = GetFont(font_id); font)
            return font->GetLineHeight();
        return 0.f;
    }

    Font* FontManager::GetFont(StringID font_id)
    {
        if (auto it = m_Fonts.find(font_id); it == m_Fonts.end())
        {
            if (!LoadFont(font_id))
            {
                return m_DefaultFont.get();
            }
        }
        return &m_Fonts[font_id];
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

    std::optional<FontManager::GlyphData> FontManager::LoadGlyph(Font* font, msdfgen::GlyphIndex glyph_index)
    {
        auto shape = font->LoadShape(glyph_index);
        if (!shape)
            return std::nullopt;

        // 1px padding in atlas to avoid bleeding from other glyphs
        const auto rect = m_Packer->Pack({ shape->width + 1, shape->height + 1 });
        if (!rect)
            return std::nullopt;

        const auto glyph = font->GenerateGlyph(*shape);
        if (!glyph)
            return std::nullopt;

        m_Atlas.Blit(glyph->bitmap.data(), rect->x, rect->y, shape->width, shape->height, glyph->stride);

        const auto bounds = shape->shape.getBounds();
        return GlyphData{
            .uv_offset = { static_cast<f32>(rect->x) / ATLAS_SIZE, static_cast<f32>(rect->y) / ATLAS_SIZE },
            .uv_scale = { static_cast<f32>(shape->width) / ATLAS_SIZE, static_cast<f32>(shape->height) / ATLAS_SIZE },
            .offset = shape->offset / ATLAS_SIZE,
        };
    }
}  // namespace fw