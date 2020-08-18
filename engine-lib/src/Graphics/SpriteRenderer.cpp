#include "SpriteRenderer.h"
#include "TextureManager.h"
#include "Framework.h"
#include <d3d11.h>
#include <execution>

namespace fw
{
	SpriteRenderer::SpriteRenderer() : m_CurrentCamera(nullptr)
	{
	}

	SpriteRenderer::~SpriteRenderer()
	{
	}

	bool SpriteRenderer::Init(const Window* window)
	{
		m_Window = window;

		if (!m_SpriteShader.Load(Shader::Vertex | Shader::Pixel, "assets/shaders/sprite.hlsl"))
			return false;

		m_ConstantBuffer.Init(sizeof(ConstantBufferData), BufferUsage::Dynamic, BufferType::Constant, 0, &m_ConstantBufferData);

		f32 vertices[] = {
			// pos      // uv
			0.0f, 0.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 1.0f,

			1.0f, 0.0f, 1.0f, 0.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			0.0f, 1.0f, 0.0f, 1.0f
		};

		m_VertexBuffer.Init(sizeof(f32) * sizeof(vertices), BufferUsage::Immutable, BufferType::Vertex, sizeof(f32) * 4, vertices);

		m_Sampler.Init(Sampler::Filter::Linear, Sampler::Address::Clamp);

		return true;
	}

	void SpriteRenderer::Submit(const SpriteCommand& sprite)
	{
		m_SpriteCommands.push_back(sprite);
	}	
	
	void SpriteRenderer::Submit(const SetCameraCommand& command)
	{
		m_CurrentCamera = command.camera;
	}

	void SpriteRenderer::Render()
	{
		auto* context = Framework::GetContext();
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		m_SpriteShader.Bind();
		m_VertexBuffer.Bind();
		m_Sampler.Bind(0);

		std::sort(std::execution::par, m_SpriteCommands.begin(), m_SpriteCommands.end(), [](auto a, auto b) {
			return a.layer > b.layer;
		});

		for (auto& sprite : m_SpriteCommands)
		{
			auto& tex = TextureManager::Get()->GetTexture(sprite.texture);
			tex.Bind(0);

			UpdateConstantBuffer(sprite, tex);
			m_ConstantBuffer.Bind(0);

			context->Draw(6, 0);
		}

		m_CurrentCamera = nullptr;

		m_SpriteCommands.clear();
		m_SpriteShader.Unbind();
		m_Sampler.Unbind(0);
	}

	void SpriteRenderer::UpdateConstantBuffer(const SpriteCommand& sprite, const Texture& texture)
	{
		auto& size = m_Window->GetSize();
		Vec2f tex_size = { (f32)texture.GetSize().x, (f32)texture.GetSize().y };
		Vec3f position = { sprite.position.x - (sprite.origin.x * tex_size.x), sprite.position.y - (sprite.origin.y * tex_size.y), 0 };

		if (m_CurrentCamera)
			m_ConstantBufferData.view_projection = sprite.world_space ? m_CurrentCamera->GetViewProjectionMatrix() : m_CurrentCamera->GetProjectionMatrix();
		else
			m_ConstantBufferData.view_projection = Mat4f::CreateOrthographicProjection(0, m_Window->GetSize().x, m_Window->GetSize().y, 0, -1, 1);

		m_ConstantBufferData.color = sprite.color;
		m_ConstantBufferData.position = sprite.position;
		m_ConstantBufferData.offset = Vec2f(sprite.origin.x * tex_size.x, sprite.origin.y * tex_size.y);
		m_ConstantBufferData.scale = sprite.scale;
		m_ConstantBufferData.size = tex_size;
		m_ConstantBufferData.resolution = Vec2f(size.x, size.y);
		m_ConstantBufferData.rotation = Radians(-sprite.rotation);
		m_ConstantBufferData.time = 0;

		m_ConstantBuffer.SetData(m_ConstantBufferData);
	}
}