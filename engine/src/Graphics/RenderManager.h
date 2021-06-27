#pragma once

#include "Core/Types.h"
#include "RenderCommand.h"
#include "SpriteRenderer.h"

namespace fw
{
	class RenderManager
	{
	public:
		RenderManager();
		~RenderManager();

		bool Init(std::shared_ptr<Window> window);

		void Submit(const RenderCommand& command);
		void Render(f32 dt, f32 total_time);

		std::shared_ptr<Texture> GetRenderTexture();
		void Resize(u32 width, u32 height);

	private:
		std::shared_ptr<Texture> m_RenderTexture;
		std::shared_ptr<Window> m_Window;
		SpriteRenderer m_SpriteRenderer;
		std::vector<RenderCommand> m_RenderCommands;
	};
}  // namespace fw