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

	void RenderManager::OnEvent(const Event& e)
	{
		m_SpriteRenderer.OnEvent(e);
	}

	void RenderManager::Update(f32 dt, f32 total_time)
	{
		SpriteCommand cmd;
		cmd.color = { 1, 1, 1, 1 };
		cmd.origin = { 0.5f, 0.5f };
		cmd.scale = { 1.0f, 1.0f };
		cmd.rotation = 0.0f;
		cmd.position = { (m_Window->GetSize().x / 2.0f) + sin(total_time) * 500.0f, m_Window->GetSize().y / 2.0f, 0, 1 };
		Submit(cmd);

		m_SpriteRenderer.Update(dt, total_time);
	}

	void RenderManager::Submit(const RenderCommand& command)
	{
		m_RenderCommands.push_back(command);
	}

	void RenderManager::Render()
	{
		for (auto& command : m_RenderCommands)
		{
			std::visit(overload{
				[&](SpriteCommand sprite) { m_SpriteRenderer.Submit(sprite); }
				}, command);
		}
		m_RenderCommands.clear();

		m_SpriteRenderer.Render();
	}
}