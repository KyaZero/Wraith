#pragma once
#include "Core\Math\Mat4.h"
#include "OrthographicCamera.h"
#include "Window\Window.h"
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
		void Submit(const SetCameraCommand& command);
		void Render();

	private:
		const Window* m_Window;

		OrthographicCamera* m_CurrentCamera;

		Shader m_SpriteShader;

		Buffer m_VertexBuffer;

		struct ConstantBufferData
		{
			Mat4f view_projection;
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

		void UpdateConstantBuffer(const SpriteCommand& sprite);

		std::vector<SpriteCommand> m_SpriteCommands;
		Sampler m_Sampler;
	};
}