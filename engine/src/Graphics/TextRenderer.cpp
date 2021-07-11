#include "TextRenderer.h"

#include <imgui/imgui.h>

#include "Graphics/DXUtil.h"
#include "Graphics/Framework.h"

namespace fw
{
    TextRenderer::TextRenderer()
    { }

    TextRenderer::~TextRenderer()
    { }

    bool TextRenderer::Init()
    {
        if (!m_FontManager.Init())
            return false;

        if (!m_TextShader.Load(Shader::Vertex | Shader::Pixel, "assets/engine/shaders/text.hlsl"))
            return false;

        // clang-format off
        f32 vertices[] = {
            // pos      // uv
            0.0f, 0.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
        };
        // clang-format on

        u32 indices[] = { 0, 2, 1, 1, 2, 3 };

        m_VertexBuffer.Init(
            sizeof(f32) * sizeof(vertices), BufferUsage::Immutable, BufferType::Vertex, sizeof(f32) * 4, vertices);
        m_IndexBuffer.Init(
            sizeof(u32) * sizeof(indices), BufferUsage::Immutable, BufferType::Index, sizeof(u32), indices);

        m_Sampler.Init(Sampler::Filter::Linear, Sampler::Address::Clamp);

        return true;
    }

    void TextRenderer::Submit(const TextCommand& command)
    {
        m_TextCommands[NEXT_FRAME].push_back(command);
    }
    void TextRenderer::Render()
    {
        for (const auto& command : m_TextCommands[CURRENT_FRAME])
        {
            for (const auto& c : command.text)
            {
                m_FontManager.GetGlyph(command.font_id, c);
            }
        }

        m_TextShader.Bind();
        m_Sampler.Bind(0);
        m_FontManager.GetAtlas().Bind(0);
        m_IndexBuffer.Bind();
        m_VertexBuffer.Bind();

        Framework::GetContext()->DrawIndexed(6, 0, 0);

        m_TextShader.Unbind();
        m_Sampler.Unbind(0);
    }

    void TextRenderer::Flip()
    {
        m_TextCommands[CURRENT_FRAME].clear();
        std::swap(m_TextCommands[CURRENT_FRAME], m_TextCommands[NEXT_FRAME]);
    }
}  // namespace fw