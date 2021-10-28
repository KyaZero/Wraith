#pragma once

#include <dubu_rect_pack/dubu_rect_pack.hpp>
#include <msdfgen/msdfgen-ext.h>
#include <msdfgen/msdfgen.h>

#include "Graphics/BlendState.h"
#include "Graphics/Buffer.h"
#include "Graphics/RenderCommand.h"
#include "Graphics/Sampler.h"
#include "Graphics/Shader.h"
#include "Graphics/Text/FontManager.h"
#include "Graphics/Texture.h"

namespace Wraith
{
    class TextRenderer
    {
        constexpr static u32 MAX_INSTANCES = 1024;
        struct InstanceData
        {
            Vec4f text_color;          // Text Color
            Vec2f glyph_uv_offset;     // UV Offset to the glyph in the atlas
            Vec2f glyph_uv_size;       // UV Size of the glyph in the atlas
            Vec2f baseline_offset;     // Offset for the glyph to land on the baseline
            Vec2f character_position;  // Position of the character in the text
            Vec2f screen_position;     // Position of the text on the screen
            float text_scale;          // Scale of the text
            float blend_mode;          // Premultiplied alpha blendmode (Additive 0 - 1 AlphaBlend)
        };
        struct ConstantBufferData
        {
            Mat4f projection;
            float pixel_range;
            float font_size;
            float atlas_size;
            float _padding;
        };

    public:
        TextRenderer();
        ~TextRenderer();

        bool Init();

        void Submit(const TextCommand& command);

        void Render(const Vec2f& viewport_size);

        void Flip();

    private:
        std::vector<TextCommand> m_TextCommands[FRAME_COUNT];
        std::vector<char> m_FontBlob;

        Shader m_TextShader;
        Buffer m_IndexBuffer;
        Buffer m_VertexBuffer;
        Buffer m_InstanceBuffer;
        Buffer m_ConstantBuffer;
        Sampler m_Sampler;
        BlendState m_BlendState;

        FontManager m_FontManager;
    };
}  // namespace Wraith