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
            Vec4f color;
            Vec2f uv_offset;
            Vec2f uv_scale;
            Vec2f offset;
            Vec2f position;
            float text_scale;
            float blend_mode;
            Vec2f screen_position;
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