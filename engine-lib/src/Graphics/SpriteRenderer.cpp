#include "SpriteRenderer.h"
#include "Framework.h"
#include <d3d11.h>
#include <execution>

namespace fw
{
	SpriteRenderer::SpriteRenderer() : m_Camera()
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

		m_Texture.LoadFromFile("assets/textures/test.jpg");
		m_Sampler.Init(Sampler::Filter::Linear, Sampler::Address::Clamp);

		m_Camera.Init(m_Window->GetSize().x, m_Window->GetSize().y);
		return true;
	}

	void SpriteRenderer::OnEvent(const Event& e)
	{
		m_Camera.OnEvent(e);
	}

	void SpriteRenderer::Update(f32 dt, f32 total_time)
	{
		m_Camera.Update(dt);
	}

	void SpriteRenderer::Submit(const SpriteCommand& sprite)
	{
		m_SpriteCommands.push_back(sprite);
	}

	void SpriteRenderer::Render()
	{
		auto* context = Framework::GetContext();
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		m_SpriteShader.Bind();
		m_VertexBuffer.Bind();
		m_Sampler.Bind(0);

		std::sort(std::execution::par, m_SpriteCommands.begin(), m_SpriteCommands.end(), [](auto a, auto b) {
			return a.position.z > b.position.z;
			});

		for (auto& sprite : m_SpriteCommands)
		{
			m_Texture.Bind(0);

			UpdateConstantBuffer(sprite);
			m_ConstantBuffer.Bind(0);

			context->Draw(6, 0);
		}

		m_SpriteCommands.clear();
		m_SpriteShader.Unbind();
		m_Sampler.Unbind(0);
	}

	void SpriteRenderer::UpdateConstantBuffer(const SpriteCommand& sprite)
	{
		auto& size = m_Window->GetSize();
		Vec2f tex_size = { (f32)m_Texture.GetSize().x, (f32)m_Texture.GetSize().y };
		Vec3f position = { sprite.position.x - (sprite.origin.x * tex_size.x), sprite.position.y - (sprite.origin.y * tex_size.y), 0 };

		m_ConstantBufferData.view_projection = sprite.world_space ? m_Camera.GetCamera().GetViewProjectionMatrix() : m_Camera.GetCamera().GetProjectionMatrix(); //Mat4f::CreateOrthographicProjection(0.0f, size.x, size.y, 0.0f, -1.0f, 1.0f);
		m_ConstantBufferData.color = sprite.color;
		m_ConstantBufferData.position = sprite.position.xy;
		m_ConstantBufferData.offset = Vec2f(sprite.origin.x * tex_size.x, sprite.origin.y * tex_size.y);
		m_ConstantBufferData.scale = sprite.scale;
		m_ConstantBufferData.size = tex_size;
		m_ConstantBufferData.resolution = Vec2f(size.x, size.y);
		m_ConstantBufferData.rotation = Radians(-sprite.rotation);
		m_ConstantBufferData.time = 0;

		m_ConstantBuffer.SetData(m_ConstantBufferData);
	}
}