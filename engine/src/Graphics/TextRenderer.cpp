#include "TextRenderer.h"

#include <imgui/imgui.h>

#include "Graphics/DXUtil.h"
#include "Graphics/Framework.h"

namespace fw
{
    TextRenderer::TextRenderer()
        : m_Packer(ATLAS_SIZE, ATLAS_SIZE)
    { }

    TextRenderer::~TextRenderer()
    {
        msdfgen::destroyFont(m_FontHandle);
        msdfgen::deinitializeFreetype(m_FreetypeHandle);
    }

    bool TextRenderer::Init()
    {
        m_Atlas.Create(TextureCreateInfo{
            .size = { ATLAS_SIZE, ATLAS_SIZE },
            .format = ImageFormat::R32G32B32A32_FLOAT,
            .render_target = false,
            .cpu_access = D3D11_CPU_ACCESS_WRITE,
        });
        SetDebugObjectName(m_Atlas.GetTexture(), "FontAtlas");

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

        m_FreetypeHandle = msdfgen::initializeFreetype();
        if (!m_FreetypeHandle)
            return false;

        // m_FontHandle = msdfgen::loadFont(m_FreetypeHandle, "assets/engine/fonts/Roboto-Regular.ttf");
        m_FontHandle = msdfgen::loadFont(m_FreetypeHandle, "c:/windows/fonts/arial.ttf");
        if (!m_FontHandle)
            return false;

        msdfgen::getFontMetrics(m_FontMetrics, m_FontHandle);
        m_FontScale = (FONT_SIZE / m_FontMetrics.emSize);

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
                if (m_Rects.find(c) == m_Rects.end())
                {
                    LoadGlyph(c);
                }
            }
        }

        m_TextShader.Bind();
        m_Sampler.Bind(0);
        m_Atlas.Bind(0);
        m_IndexBuffer.Bind();
        m_VertexBuffer.Bind();

        Framework::GetContext()->DrawIndexed(6, 0, 0);

        m_TextShader.Unbind();
        m_Sampler.Unbind(0);
    }

    void TextRenderer::LoadGlyph(u16 c)
    {
        double advance;
        msdfgen::Shape shape;
        if (!msdfgen::loadGlyph(shape, m_FontHandle, c, &advance))
        {
            return;
        }
        if (shape.edgeCount() == 0)
        {
            return;
        }
        shape.normalize();

        const i32 range = 2;
        auto bounds = shape.getBounds();
        i32 w = std::ceil((bounds.r - bounds.l + range * 2) * m_FontScale);
        i32 h = std::ceil((bounds.t - bounds.b + range * 2) * m_FontScale);

        const auto rect = m_Packer.Pack({ static_cast<u32>(w + 1), static_cast<u32>(h + 1) });
        if (!rect)
        {
            return;
        }

        m_Rects[c] = *rect;

        msdfgen::edgeColoringSimple(shape, 3.0);
        msdfgen::Bitmap<float, 4> msdf(w, h);
        msdfgen::generateMTSDF(msdf, shape, { m_FontScale, { range - bounds.l, range - bounds.b } }, range);
        // msdfgen::savePng(msdf, std::format("saved/{}.png", static_cast<u32>(c)).c_str());

        m_Atlas.Blit(
            reinterpret_cast<u8*>(msdf(0, 0)), rect->x, rect->y, msdf.width(), msdf.height(), msdf.width() * 16);
    }

    void TextRenderer::Flip()
    {
        m_TextCommands[CURRENT_FRAME].clear();
        std::swap(m_TextCommands[CURRENT_FRAME], m_TextCommands[NEXT_FRAME]);
    }
}  // namespace fw