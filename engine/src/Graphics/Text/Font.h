#pragma once

#include <fribidi.h>
#include <hb.h>
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
        struct ShapedGlyph
        {
            u32 glyph_id;
            Vec2f position;
        };
        struct DisplayData
        {
            std::vector<ShapedGlyph> shapedGlyphs;
            Vec2f size;
        };

    public:
        Font(msdfgen::FreetypeHandle* freetypeHandle = nullptr);

        bool Init(std::filesystem::path fontPath);
        void Release();

        std::optional<ShapeData> LoadShape(u32 c);
        std::optional<GlyphData> GenerateGlyph(ShapeData& shape_data);

        DisplayData ShapeText(std::string_view text, i32 direction);

        f32 GetLineHeight() const
        {
            return m_LineHeight;
        }

    private:
        struct
        {
            msdfgen::FreetypeHandle* handle = nullptr;
            msdfgen::FontHandle* font_handle = nullptr;
        } m_Freetype;

        struct
        {
            hb_buffer_t* buffer = nullptr;
            hb_blob_t* blob = nullptr;
            hb_face_t* face = nullptr;
            hb_font_t* font = nullptr;
            Vec2f scale;
        } m_HarfBuzz;

        struct
        {
            std::vector<FriBidiChar> logical;
            std::vector<FriBidiCharType> visual;
        } m_FriBidi;

        f32 m_FontScale;
        f32 m_LineHeight;
    };
}  // namespace fw