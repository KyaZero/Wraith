#pragma once

#include <dubu_rect_pack/dubu_rect_pack.hpp>
#include <msdfgen/msdfgen-ext.h>
#include <msdfgen/msdfgen.h>

#include "Graphics/Buffer.h"
#include "Graphics/RenderCommand.h"
#include "Graphics/Sampler.h"
#include "Graphics/Shader.h"
#include "Graphics/Text/FontManager.h"
#include "Graphics/Texture.h"

namespace fw
{
    class TextRenderer
    {
    public:
        TextRenderer();
        ~TextRenderer();

        bool Init();

        void Submit(const TextCommand& command);

        void Render();

        void Flip();

    private:
        std::vector<TextCommand> m_TextCommands[FRAME_COUNT];
        std::vector<char> m_FontBlob;

        Shader m_TextShader;
        Buffer m_IndexBuffer;
        Buffer m_VertexBuffer;
        Sampler m_Sampler;

        FontManager m_FontManager;
    };
}  // namespace fw