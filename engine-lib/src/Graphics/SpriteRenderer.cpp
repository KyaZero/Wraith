#include "SpriteRenderer.h"
#include "TextureManager.h"
#include "Framework.h"
#include <d3d11.h>
#include <execution>
#include <array>

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
			1.0f, 1.0f, 1.0f, 1.0f,
		};

		u32 indices[] = {
			0, 1, 2, 1, 3, 2
		};

		m_VertexBuffer.Init(sizeof(f32) * sizeof(vertices), BufferUsage::Immutable, BufferType::Vertex, sizeof(f32) * 4, vertices);
		m_IndexBuffer.Init(sizeof(u32) * sizeof(indices), BufferUsage::Immutable, BufferType::Index, sizeof(u32), indices);
		m_InstanceBuffer.Init(InstanceCount * sizeof(InstanceData), BufferUsage::Dynamic, BufferType::Structured, sizeof(InstanceData));

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

		UpdateConstantBuffer();
		m_ConstantBuffer.Bind(0);

		m_SpriteShader.Bind();
		m_IndexBuffer.Bind();
		m_VertexBuffer.Bind();
		m_Sampler.Bind(0);

		std::sort(std::execution::par, m_SpriteCommands.begin(), m_SpriteCommands.end(), [](auto a, auto b) {
			if(a.texture == b.texture)
				return a.layer > b.layer;
			return a.texture > b.texture;
		});

		std::map<TextureID, std::vector<InstanceData>> instances;

		TextureID current_texture_id = 0;
		Vec2f current_texture_size = { 0, 0 };
		for (auto& sprite : m_SpriteCommands)
		{
			//caching
			if (sprite.texture != current_texture_id)
			{
				current_texture_id = sprite.texture;
				auto& tex = TextureManager::Get()->GetTexture(current_texture_id);
				current_texture_size = { (f32)tex.GetSize().x, (f32)tex.GetSize().y };
			}

			InstanceData data;
			data.color = sprite.color;
			data.offset = Vec2f(sprite.origin.x * current_texture_size.x, sprite.origin.y * current_texture_size.y);
			data.position = sprite.position;
			data.rotation = Radians(-sprite.rotation);
			data.scale = sprite.scale;
			data.size = current_texture_size;
			data.world_space = sprite.world_space;

			instances[sprite.texture].push_back(data);
		}

		for (auto& sprites : instances)
		{
			u32 batches = 1 + (sprites.second.size() / InstanceCount);
			for (i32 i = 0; i < batches; ++i)
			{
				auto& tex = TextureManager::Get()->GetTexture(sprites.first);
				tex.Bind(0);

				i32 num_instances = Min(InstanceCount, (i32)sprites.second.size() - (InstanceCount * i));

				m_InstanceBuffer.SetData(&sprites.second[0] + (InstanceCount * i), num_instances * sizeof(InstanceData));
				m_InstanceBuffer.Bind(1);

				context->DrawIndexedInstanced(6, num_instances, 0, 0, 0);
			}
		}

		m_CurrentCamera = nullptr;

		m_SpriteCommands.clear();
		m_SpriteShader.Unbind();
		m_Sampler.Unbind(0);
	}

	void SpriteRenderer::UpdateConstantBuffer(/*const SpriteCommand& sprite, const Texture& texture*/)
	{
		auto& size = m_Window->GetSize();

		if (m_CurrentCamera)
		{
			m_ConstantBufferData.view_projection = m_CurrentCamera->GetViewProjectionMatrix();
			m_ConstantBufferData.projection = m_CurrentCamera->GetProjectionMatrix();
		}
		else
		{
			m_ConstantBufferData.view_projection = Mat4f::CreateOrthographicProjection(0, m_Window->GetSize().x, m_Window->GetSize().y, 0, -1, 1);
		}

		m_ConstantBufferData.resolution = Vec2f(size.x, size.y);
		m_ConstantBufferData.time = 0;

		m_ConstantBuffer.SetData(m_ConstantBufferData);
	}
}