#include "TextRenderer.h"

#include <imgui/imgui.h>

#include "Graphics/DXUtil.h"
#include "Graphics/Framework.h"

namespace Wraith
{
    TextRenderer::TextRenderer() { }

    TextRenderer::~TextRenderer() { }

    bool TextRenderer::Init()
    {
        if (!m_FontManager.Init())
            return false;

        if (!m_TextShader.Load(Shader::Vertex | Shader::Pixel, "assets/engine/shaders/text.hlsl"))
            return false;

        if (!m_BlendState.Create(BlendMode::One, BlendMode::InvSrcAlpha, BlendMode::One, BlendMode::InvSrcAlpha))
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

        u32 indices[] = { 2, 3, 1, 2, 1, 0 };

        //m_VertexBuffer.Init(sizeof(f32) * sizeof(vertices), BufferUsage::Immutable, BufferType::Vertex, sizeof(f32) * 4, vertices);
        //m_IndexBuffer.Init(sizeof(u32) * sizeof(indices), BufferUsage::Immutable, BufferType::Index, sizeof(u32), indices);
        //m_InstanceBuffer.Init(MAX_INSTANCES * sizeof(InstanceData), BufferUsage::Dynamic, BufferType::Structured, sizeof(InstanceData));
        //m_ConstantBuffer.Init(sizeof(ConstantBufferData), BufferUsage::Dynamic, BufferType::Constant);

        m_Sampler.Init(Sampler::Filter::Linear, Sampler::Address::Clamp);

        return true;
    }

    void TextRenderer::Render(const Vec2f& viewport_size)
    {
        std::vector<InstanceData> instances;
        for (const auto& command : GetCurrentCommands())
        {
            std::string_view text = command.text;

            std::vector<Font::DisplayData> displayData;
            while (!text.empty())
            {
                auto split_pos = text.find('\n');
                displayData.push_back(m_FontManager.ShapeText(command.font_id, text.substr(0, split_pos), command.direction));

                if (split_pos == text.npos)
                    break;
                text = text.substr(split_pos + 1);
            }

            const f32 line_height = m_FontManager.GetLineHeight(command.font_id);
            const f32 total_height = line_height * displayData.size();
            f32 total_width = 0.f;
            for (const auto& [_, size] : displayData)
            {
                total_width = Max(total_width, size.x);
            }

            float baseline = line_height;
            for (const auto& [shaped_glyphs, size] : displayData)
            {
                for (const auto& sg : shaped_glyphs)
                {
                    const auto glyph = m_FontManager.GetGlyph(command.font_id, sg.glyph_index);
                    if (!glyph)
                        continue;

                    instances.push_back(InstanceData{
                        .text_color = command.color,
                        .glyph_uv_offset = glyph->uv_offset,
                        .glyph_uv_size = glyph->uv_scale,
                        .baseline_offset = glyph->offset,
                        .character_position = sg.position - Vec2f(size.x * command.justification, baseline) +
                                              Vec2f(total_width * command.justification, total_height * command.alignment),
                        .screen_position = command.screen_position,
                        .text_scale = static_cast<f32>(command.font_size),
                        .blend_mode = command.blend_mode,
                    });
                }
                baseline += line_height;
            }
        }

        //m_ConstantBuffer.SetData(ConstantBufferData{
        //    .projection = Mat4f::CreateOrthographicProjection(0, viewport_size.x, -viewport_size.y, 0, 0.f, 1.f),
        //    .pixel_range = Font::SDF_RANGE,
        //    .font_size = Font::FONT_SIZE,
        //    .atlas_size = FontManager::ATLAS_SIZE,
        //});

        //m_ConstantBuffer.Bind(0);
        m_TextShader.Bind();
        m_Sampler.Bind(0);
        m_FontManager.GetAtlas().Bind(0);
        //m_IndexBuffer.Bind();
        //m_VertexBuffer.Bind();
        m_BlendState.Bind();

        const u32 batches = (static_cast<u32>(instances.size()) / MAX_INSTANCES) + 1;
        for (u32 i = 0u; i < batches; ++i)
        {
            //const auto num_instances = std::min(MAX_INSTANCES, static_cast<u32>(instances.size()) - MAX_INSTANCES * i);

            //m_InstanceBuffer.SetData(instances.data() + MAX_INSTANCES * i, num_instances * sizeof(InstanceData));

            //m_InstanceBuffer.Bind(1);
            // Framework::GetContext().DrawIndexedInstanced(6, num_instances, 0, 0, 0);
        }

        m_BlendState.Unbind();
        m_TextShader.Unbind();
        m_Sampler.Unbind(0);
    }
}  // namespace Wraith