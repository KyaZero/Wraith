#pragma once
#include "..\Core\Math\Mat4.h"
#include "..\Window\Window.h"
#include "Shader.h"
#include "Buffer.h"
#include "Sampler.h"
#include "RenderCommand.h"
#include "Texture.h"

namespace fw
{
	class SpriteRenderer
	{
	public:
		SpriteRenderer();
		~SpriteRenderer();

		bool Init(const Window* window);

		void Submit(const SpriteCommand& sprite);
		void Render(f32 dt);

	private:
		const Window* m_Window;

		Shader m_SpriteShader;

		Buffer m_VertexBuffer;

		struct ConstantBufferData
		{
			Mat4f projection_matrix;
			Vec4f color;
			Vec2f position;
			Vec2f offset;
			Vec2f scale;
			Vec2f size;
			Vec2f resolution;
			f32 rotation;
			f32 time;
		} m_ConstantBufferData;
		Buffer m_ConstantBuffer;

		void UpdateConstantBuffer(const SpriteCommand& sprite, f32 dt);

		std::vector<SpriteCommand> m_SpriteCommands;
		Texture m_Texture;
		Sampler m_Sampler;
	};
}