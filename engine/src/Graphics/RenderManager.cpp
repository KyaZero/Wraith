#include "RenderManager.h"
#include "Framework.h"

// helper template stuff for std::visit + std::variant usage :)
// see link for details
// https://www.bfilipek.com/2018/06/variant.html#overload
template<class... Ts> struct overload : Ts... { using Ts::operator()...; };

namespace fw
{
	RenderManager::RenderManager()
	{
	}

	RenderManager::~RenderManager()
	{
	}

	bool RenderManager::Init(std::shared_ptr<Window> window)
	{
		m_Window = window;

		if (!m_SpriteRenderer.Init(m_Window))
			return false;

		m_RenderTexture = std::make_shared<Texture>();
		m_RenderTexture->Create(m_Window->GetSize());

		Window::RegisterResizeCallback(this, [&](auto w, auto h) { Resize(w, h); });
		return true;
	}

	void RenderManager::Submit(const RenderCommand& command)
	{
		m_RenderCommands.push_back(command);
	}

	void RenderManager::Render(f32 dt, f32 total_time)
	{
		m_RenderTexture->Clear({ 0.2f, 0.0f, 0.2f, 0.2f });
		m_RenderTexture->SetAsActiveTarget();

		for (auto& command : m_RenderCommands)
		{
			std::visit(overload{
				[&](SpriteCommand sprite) { m_SpriteRenderer.Submit(sprite); },
				[&](SetCameraCommand camera) { m_SpriteRenderer.Submit(camera); }
				}, command);
		}
		m_RenderCommands.clear();

		Framework::BeginEvent("Render Sprites");
		m_SpriteRenderer.Render(dt, total_time);
		Framework::EndEvent();

		m_RenderTexture->UnsetActiveTarget();
	}

	std::shared_ptr<Texture> RenderManager::GetRenderTexture()
	{
		return m_RenderTexture;
	}

	void RenderManager::Resize(u32 width, u32 height)
	{
		m_RenderTexture->Resize(m_Window->GetSize());
	}
}