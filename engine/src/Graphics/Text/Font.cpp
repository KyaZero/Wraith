#include "Font.h"

namespace fw
{
    Font::Font(msdfgen::FreetypeHandle* freetypeHandle)
        : m_FreetypeHandle(freetypeHandle)
        , m_FontHandle(nullptr)
    { }

    bool Font::Init(std::filesystem::path fontPath)
    {
        m_FontHandle = msdfgen::loadFont(m_FreetypeHandle, fontPath.generic_string().c_str());
        if (!m_FontHandle)
            return false;

        msdfgen::getFontMetrics(m_FontMetrics, m_FontHandle);
        m_FontScale = (FONT_SIZE / m_FontMetrics.emSize);

        double space_advance, tab_advance;
        msdfgen::getFontWhitespaceWidth(space_advance, tab_advance, m_FontHandle);

        m_SpaceWidth = static_cast<f32>(space_advance * m_FontScale);

        return true;
    }

    void Font::Release()
    {
        if (m_FontHandle)
        {
            msdfgen::destroyFont(m_FontHandle);
            m_FontHandle = nullptr;
        }
    }

    std::optional<Font::ShapeData> Font::LoadShape(u16 c)
    {
        double advance;
        msdfgen::Shape shape;
        if (!msdfgen::loadGlyph(shape, m_FontHandle, c, &advance))
        {
            return std::nullopt;
        }
        if (shape.edgeCount() == 0)
        {
            return std::nullopt;
        }
        shape.normalize();

        const auto bounds = shape.getBounds();
        u32 w = std::ceil((bounds.r - bounds.l + SDF_RANGE) * m_FontScale);
        u32 h = std::ceil((bounds.t - bounds.b + SDF_RANGE) * m_FontScale);

        return ShapeData{
            .width = w,
            .height = h,
            .shape = shape,
            .advance = advance * m_FontScale,
            .offset = Vec2f(bounds.l * m_FontScale, bounds.b * m_FontScale),
        };
    }

    std::optional<Font::GlyphData> Font::GenerateGlyph(ShapeData& shape_data)
    {
        auto& [width, height, shape, advance, offset] = shape_data;

        const auto bounds = shape.getBounds();

        msdfgen::edgeColoringSimple(shape, 3.0);
        msdfgen::Bitmap<float, 4> msdf(width, height);
        msdfgen::generateMTSDF(msdf, shape, { m_FontScale, { SDF_RANGE - bounds.l, SDF_RANGE - bounds.b } }, SDF_RANGE);

        GlyphData glyph_data;
        glyph_data.stride = shape_data.width * sizeof(float) * 4;
        glyph_data.bitmap = std::vector<u8>(reinterpret_cast<u8*>(msdf(0, 0)),
                                            reinterpret_cast<u8*>(msdf(0, 0)) + shape_data.height * glyph_data.stride);
        return glyph_data;
    }

}  // namespace fw