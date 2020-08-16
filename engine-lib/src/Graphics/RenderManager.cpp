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
		if (!m_SpriteRenderer.Init(window))
			return false;

		return true;
	}

	void RenderManager::Submit(const RenderCommand& command)
	{
		m_RenderCommands.push_back(command);
	}

	static std::vector<SpriteCommand> commands;

	void RenderManager::Render()
	{
		static bool done = false;
		if (!done)
		{
			for (size_t i = 0; i < 20; i++)
			{
				SpriteCommand cmd;
				cmd.color = { Rand(),Rand(),Rand(),1 };
				cmd.uv = { 0,0,1,1 };
				cmd.position = { Rand11(), Rand11(), RandomRange(1.f, 5.f), 1 };
				commands.push_back(cmd);
			}
			done = true;
		}

		for(auto& cmd : commands)
			Submit(cmd);

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