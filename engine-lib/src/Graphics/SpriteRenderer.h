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
		void Render();

	private:
		const Window* m_Window;

		Shader m_SpriteShader;
		Buffer m_SpriteBuffer;

		struct ConstantBufferData
		{
			Mat4f projection_matrix;
			Vec4f resolution;
			Vec2f time;
			Vec2f size;
		} m_ConstantBufferData;
		Buffer m_ConstantBuffer;

		void UpdateConstantBuffer();

		std::vector<SpriteCommand> m_SpriteCommands;
		Texture m_Texture;
		Sampler m_Sampler;
	};
}