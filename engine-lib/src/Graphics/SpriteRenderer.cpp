#include "SpriteRenderer.h"
#include "Framework.h"
#include <d3d11.h>
#include <execution>

namespace fw
{
	SpriteRenderer::SpriteRenderer()
	{
	}

	SpriteRenderer::~SpriteRenderer()
	{
	}

	bool SpriteRenderer::Init(const Window* window)
	{
		m_Window = window;

		if (!m_SpriteShader.Load(Shader::Vertex | Shader::Geometry | Shader::Pixel, "assets/shaders/sprite.hlsl"))
			return false;

		m_SpriteBuffer.Init(sizeof(SpriteCommand), BufferUsage::Dynamic, BufferType::Vertex, 0);
		m_ConstantBuffer.Init(sizeof(ConstantBufferData), BufferUsage::Dynamic, BufferType::Constant, 0, &m_ConstantBufferData);

		m_Texture.LoadFromFile("assets/textures/default.png");
		m_Sampler.Init(Sampler::Filter::Linear, Sampler::Address::Clamp);
		return true;
	}

	void SpriteRenderer::Submit(const SpriteCommand& sprite)
	{
		m_SpriteCommands.push_back(sprite);
	}

	void SpriteRenderer::Render()
	{
		m_SpriteShader.Bind();
		m_Sampler.Bind(0);

		std::sort(std::execution::par, m_SpriteCommands.begin(), m_SpriteCommands.end(), [](auto a, auto b) {
			return a.position.z > b.position.z;
		});

		for (auto& sprite : m_SpriteCommands)
		{
			auto* context = Framework::GetContext();

			m_Texture.Bind(0);

			m_SpriteBuffer.SetData(sprite);
			m_SpriteBuffer.Bind();

			UpdateConstantBuffer();
			m_ConstantBuffer.Bind(0);

			context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
			context->Draw(1, 0);
		}

		m_SpriteCommands.clear();
		m_SpriteShader.Unbind();
		m_Sampler.Unbind(0);
	}

	void SpriteRenderer::UpdateConstantBuffer()
	{
		auto& size = m_Window->GetSize();
		if (size.x == 0 || size.y == 0)
			return;

		m_ConstantBufferData.projection_matrix = Mat4f::CreatePerspectiveProjection(80.0f, (size.x / size.y), 0.1f, 1000.0f);
		m_ConstantBufferData.resolution = Vec4f(size.x, size.y, size.x / size.y, size.y / size.x);
		m_ConstantBufferData.time = Vec2f(0, 0);
		m_ConstantBufferData.size = Vec2f(128 / 100.0f, 128 / 100.0f);

		m_ConstantBuffer.SetData(m_ConstantBufferData);
	}
}
