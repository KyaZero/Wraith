#pragma once

#include "Graphics/RenderCommand.h"
#include "Graphics/Texture.h"
#include "Graphics/Shader.h"
#include "Graphics/Buffer.h"
#include "Graphics/Sampler.h"

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