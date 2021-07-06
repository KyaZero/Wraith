#include "TextRenderer.h"

#include <fstream>

#include <msdfgen/msdfgen-ext.h>
#include <msdfgen/msdfgen.h>

#include "Graphics/DXUtil.h"
#include "Graphics/Framework.h"

namespace fw
{
    bool TextRenderer::Init()
    {
        m_Atlas.Create(TextureCreateInfo{
            .size = { 256, 256 },
            .format = ImageFormat::R32G32B32A32_FLOAT,
            .render_target = false,
            .usage = D3D11_USAGE_DYNAMIC,
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

        {  // msdfgen
            auto freetypeHandle = msdfgen::initializeFreetype();
            //auto fontHandle = msdfgen::loadFont(freetypeHandle, "assets/engine/fonts/Roboto-Regular.ttf");
            auto fontHandle = msdfgen::loadFont(freetypeHandle, "c:/windows/fonts/bazzi.ttf");

            msdfgen::Shape shape;
            msdfgen::loadGlyph(shape, fontHandle, L'A');
            msdfgen::loadGlyph(shape, fontHandle, L'한');

            const i32 range = 2;
            auto bounds = shape.getBounds();
            float scale = 16.f / (bounds.t-bounds.b);
            i32 w = std::round((bounds.r-bounds.l+range*2)*scale);
            i32 h = std::round((bounds.t-bounds.b+range*2)*scale);

            msdfgen::edgeColoringSimple(shape, 3.0);
            msdfgen::Bitmap<float, 4> msdf(w, h);
            msdfgen::generateMTSDF(msdf, shape, {scale, range}, range);
            msdfgen::savePng(msdf, "output.png");

            m_Atlas.Blit(reinterpret_cast<u8*>(msdf(0, 0)), 0, 0, w, h, w*4*sizeof(float));

            msdfgen::destroyFont(fontHandle);
            msdfgen::deinitializeFreetype(freetypeHandle);
        }

        return true;
    }

    void TextRenderer::Submit(const TextCommand& command)
    {
        m_TextCommands.push_back(command);
    }
    void TextRenderer::Render()
    {
        m_TextShader.Bind();
        m_Sampler.Bind(0);
        m_Atlas.Bind(0);
        m_IndexBuffer.Bind();
        m_VertexBuffer.Bind();

        Framework::GetContext()->DrawIndexed(6, 0, 0);

        m_TextShader.Unbind();
        m_Sampler.Unbind(0);
    }
}  // namespace fw