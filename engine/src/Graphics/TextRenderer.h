#pragma once

#include <mutex>

#include <dubu_rect_pack/dubu_rect_pack.hpp>
#include <msdfgen/msdfgen-ext.h>
#include <msdfgen/msdfgen.h>

#include "Graphics/Buffer.h"
#include "Graphics/RenderCommand.h"
#include "Graphics/Sampler.h"
#include "Graphics/Shader.h"
#include "Graphics/Texture.h"

namespace fw
{
    class TextRenderer
    {
        constexpr static u32 ATLAS_SIZE = 256;
        constexpr static f32 FONT_SIZE = 20.f;

    public:
        TextRenderer();
        ~TextRenderer();

        bool Init();

        void Submit(const TextCommand& command);

        void Render();

        void LoadGlyph(u16 c);

        void Flip();

    private:
        std::vector<TextCommand> m_TextCommands[FRAME_COUNT];
        std::vector<char> m_FontBlob;

        Texture m_Atlas;
        Shader m_TextShader;
        Buffer m_IndexBuffer;
        Buffer m_VertexBuffer;
        Sampler m_Sampler;

        msdfgen::FreetypeHandle* m_FreetypeHandle;
        msdfgen::FontHandle* m_FontHandle;
        msdfgen::FontMetrics m_FontMetrics;
        f32 m_FontScale;

        dubu::rect_pack::Packer m_Packer;
        std::unordered_map<u16, dubu::rect_pack::Rect> m_Rects;
    };
}  // namespace fw