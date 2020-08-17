#pragma once
#include "SpriteRenderer.h"
#include "RenderCommand.h"

namespace fw
{
	class RenderManager
	{
	public:
		RenderManager();
		~RenderManager();

		bool Init(const Window* window);
		
		void Submit(const RenderCommand& command);
		void Render(f32 dt);

	private:
		const Window* m_Window;
		SpriteRenderer m_SpriteRenderer;
		std::vector<RenderCommand> m_RenderCommands;
	};
}