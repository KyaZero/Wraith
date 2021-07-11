#pragma once

#include <msdfgen/msdfgen-ext.h>
#include <msdfgen/msdfgen.h>

namespace fw
{
    class Font
    {
        constexpr static f32 FONT_SIZE = 20.f;
        constexpr static i32 SDF_RANGE = 2;

    public:
        struct ShapeData
        {
            u32 width;
            u32 height;
            msdfgen::Shape shape;
            double advance;
        };
        struct GlyphData
        {
            u32 width;
            u32 height;
            std::vector<u8> bitmap;
            u32 stride;
            double advance;

            void Serialize(dubu::serialize::ReadBuffer& buffer)
            {
                buffer >> width >> height >> bitmap >> stride >> advance;
            }
            void Serialize(dubu::serialize::WriteBuffer& buffer) const
            {
                buffer << width << height << bitmap << stride << advance;
            }
        };

    public:
        Font(msdfgen::FreetypeHandle* freetypeHandle = nullptr);

        bool Init(std::filesystem::path fontPath);
        void Release();

        std::optional<ShapeData> LoadShape(u16 c);
        std::optional<GlyphData> GenerateGlyph(ShapeData& shape_data);

    private:
        msdfgen::FreetypeHandle* m_FreetypeHandle;
        msdfgen::FontHandle* m_FontHandle;
        msdfgen::FontMetrics m_FontMetrics;
        f32 m_FontScale;
    };
}  // namespace fw