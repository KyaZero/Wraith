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

		bool Init(std::shared_ptr<Window> window);

		void Submit(const SpriteCommand& sprite);
		void Submit(const SetCameraCommand& command);
		void Render(f32 dt, f32 total_time);

	private:
		std::shared_ptr<Window> m_Window;

		struct RenderCamera
		{
			Mat4f projection;
			Mat4f view;
		};

		std::unique_ptr<RenderCamera> m_CurrentCamera;

		Shader m_SpriteShader;

		Buffer m_IndexBuffer;
		Buffer m_VertexBuffer;
		Buffer m_InstanceBuffer;

		struct ConstantBufferData
		{
			Mat4f view_projection;
			Mat4f projection;
			Vec2f resolution;
			f32 time;
		} m_ConstantBufferData;
		Buffer m_ConstantBuffer;

		struct InstanceData
		{
			Vec4f color;
			Vec2f position;
			Vec2f offset;
			Vec2f scale;
			Vec2f size;
			f32 rotation;
			i32 world_space;
		};

		void UpdateConstantBuffer(f32 total_time);

		//I found 1024 instances to be the best in terms of performance and drawcalls
		constexpr static u32 InstanceCount = 1024;
		std::vector<SpriteCommand> m_SpriteCommands;
		Sampler m_Sampler;
	};
}