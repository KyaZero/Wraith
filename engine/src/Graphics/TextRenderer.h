#pragma once

#include <dubu_rect_pack/dubu_rect_pack.hpp>

#include "Graphics/Buffer.h"
#include "Graphics/RenderCommand.h"
#include "Graphics/Sampler.h"
#include "Graphics/Shader.h"
#include "Graphics/Texture.h"

namespace fw
{
    class TextRenderer
    {
    public:
        bool Init();

        void Submit(const TextCommand& command);

        void Render();

    private:
        std::vector<TextCommand> m_TextCommands;
        std::vector<char> m_FontBlob;

        Texture m_Atlas;
        Shader m_TextShader;
        Buffer m_IndexBuffer;
        Buffer m_VertexBuffer;
        Sampler m_Sampler;
    };
}  // namespace fw