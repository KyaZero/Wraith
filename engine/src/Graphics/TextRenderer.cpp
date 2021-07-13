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

        if (!m_BlendState.Create(
                D3D11_BLEND_ONE, D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_ONE, D3D11_BLEND_INV_SRC_ALPHA))
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
        m_InstanceBuffer.Init(
            MAX_INSTANCES * sizeof(InstanceData), BufferUsage::Dynamic, BufferType::Structured, sizeof(InstanceData));
        m_ConstantBuffer.Init(sizeof(ConstantBufferData), BufferUsage::Dynamic, BufferType::Constant);

        m_Sampler.Init(Sampler::Filter::Linear, Sampler::Address::Clamp);

        return true;
    }

    void TextRenderer::Submit(const TextCommand& command)
    {
        m_TextCommands[NEXT_FRAME].push_back(command);
    }
    void TextRenderer::Render()
    {
        f32 advance = 0.f;
        f32 baseline = 0.f;
        std::vector<InstanceData> instances;
        for (const auto& command : m_TextCommands[CURRENT_FRAME])
        {
            for (const auto& c : command.text)
            {
                if (c == ' ' || c == '\t')
                {
                    advance += m_FontManager.GetSpaceWidth(command.font_id);
                    continue;
                }
                if (c == '\n')
                {
                    advance = 0.f;
                    baseline += m_FontManager.GetLineHeight(command.font_id);
                    continue;
                }

                auto glyph = m_FontManager.GetGlyph(command.font_id, c);
                if (!glyph)
                    continue;

                instances.push_back(InstanceData{
                    .uv_offset = glyph->uv_offset,
                    .uv_scale = glyph->uv_scale,
                    .offset = glyph->offset,
                    .position = { advance, -baseline },
                });
                advance += glyph->advance;
            }
        }

        m_ConstantBuffer.SetData(ConstantBufferData{
            .pixel_range = Font::SDF_RANGE,
            .font_size = Font::FONT_SIZE,
            .atlas_size = FontManager::ATLAS_SIZE,
        });

        m_ConstantBuffer.Bind(0);
        m_TextShader.Bind();
        m_Sampler.Bind(0);
        m_FontManager.GetAtlas().Bind(0);
        m_IndexBuffer.Bind();
        m_VertexBuffer.Bind();
        m_BlendState.Bind();

        const u32 batches = (static_cast<u32>(instances.size()) / MAX_INSTANCES) + 1;
        for (u32 i = 0u; i < batches; ++i)
        {
            const auto num_instances = std::min(MAX_INSTANCES, static_cast<u32>(instances.size()) - MAX_INSTANCES * i);

            m_InstanceBuffer.SetData(instances.data() + MAX_INSTANCES * i, num_instances * sizeof(InstanceData));

            m_InstanceBuffer.Bind(1);
            Framework::GetContext()->DrawIndexedInstanced(6, num_instances, 0, 0, 0);
        }

        m_BlendState.Unbind();
        m_TextShader.Unbind();
        m_Sampler.Unbind(0);
    }

    void TextRenderer::Flip()
    {
        m_TextCommands[CURRENT_FRAME].clear();
        std::swap(m_TextCommands[CURRENT_FRAME], m_TextCommands[NEXT_FRAME]);
    }
}  // namespace fw