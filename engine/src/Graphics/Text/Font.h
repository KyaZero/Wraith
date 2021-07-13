#pragma once

#include <msdfgen/msdfgen-ext.h>
#include <msdfgen/msdfgen.h>

namespace fw
{
    class Font
    {
    public:
        constexpr static f32 FONT_SIZE = 20.f;
        constexpr static i32 SDF_RANGE = 2;

    public:
        struct ShapeData
        {
            u32 width;
            u32 height;
            msdfgen::Shape shape;
            double advance;
            Vec2f offset;
        };
        struct GlyphData
        {
            std::vector<u8> bitmap;
            u32 stride;
        };

    public:
        Font(msdfgen::FreetypeHandle* freetypeHandle = nullptr);

        bool Init(std::filesystem::path fontPath);
        void Release();

        std::optional<ShapeData> LoadShape(u16 c);
        std::optional<GlyphData> GenerateGlyph(ShapeData& shape_data);

        f32 GetSpaceWidth() const
        {
            return m_SpaceWidth;
        }
        f32 GetLineHeight() const
        {
            return static_cast<f32>(m_FontMetrics.lineHeight * m_FontScale);
        }

    private:
        msdfgen::FreetypeHandle* m_FreetypeHandle;
        msdfgen::FontHandle* m_FontHandle;
        msdfgen::FontMetrics m_FontMetrics;
        f32 m_FontScale;
        f32 m_SpaceWidth;
    };
}  // namespace fw