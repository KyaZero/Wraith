#include "RenderManager.h"

// helper template stuff for std::visit + std::variant usage :)
// see link for details
// https://www.bfilipek.com/2018/06/variant.html#overload
template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
template<class... Ts> overload(Ts...)->overload<Ts...>;

namespace fw
{
	RenderManager::RenderManager()
	{
	}

	RenderManager::~RenderManager()
	{
	}

	bool RenderManager::Init(const Window* window)
	{
		m_Window = window;

		if (!m_SpriteRenderer.Init(m_Window))
			return false;

		return true;
	}

	void RenderManager::Submit(const RenderCommand& command)
	{
		m_RenderCommands.push_back(command);
	}

	void RenderManager::Render(f32 dt)
	{
		SpriteCommand cmd;
		cmd.color = { 1,1,1,1 };
		cmd.origin = { 0.5f, 0.5f };
		cmd.scale = { 1, 1 };
		cmd.rotation = 90.0f;
		cmd.position = { m_Window->GetSize().x * 0.5f, m_Window->GetSize().y * 0.5f, 0, 1 };
		Submit(cmd);

		for (auto& command : m_RenderCommands)
		{
			std::visit(overload{
				[&](SpriteCommand sprite) { m_SpriteRenderer.Submit(sprite); }
				}, command);
		}
		m_RenderCommands.clear();

		m_SpriteRenderer.Render(dt);
	}
}