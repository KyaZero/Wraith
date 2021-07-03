#include "RenderManager.h"

#include "Framework.h"

namespace fw
{
    RenderManager::RenderManager(Window& window)
        : m_Window(window)
        , m_SpriteRenderer(window)
    { }

    RenderManager::~RenderManager()
    { }

    bool RenderManager::Init()
    {
        if (!m_SpriteRenderer.Init())
            return false;

        m_RenderTexture = std::make_unique<Texture>();
        m_RenderTexture->Create(m_Window.GetSize());

        Window::RegisterResizeCallback(this, [&](auto w, auto h) { Resize(w, h); });
        return true;
    }

    void RenderManager::Submit(const RenderCommand& command)
    {
        m_RenderCommands.push_back(command);
    }

    void RenderManager::Render(f32 dt, f32 total_time)
    {
        m_RenderTexture->Clear({ 0.0f, 0.0f, 0.0f, 1.0f });
        m_RenderTexture->SetAsActiveTarget();

        for (auto& command : m_RenderCommands)
        {
            std::visit(overloaded{ [&](SpriteCommand sprite) { m_SpriteRenderer.Submit(sprite); },
                                   [&](SetCameraCommand camera) { m_SpriteRenderer.Submit(camera); } },
                       command);
        }
        m_RenderCommands.clear();

        Framework::BeginEvent("Render Sprites");
        m_SpriteRenderer.Render(dt, total_time);
        Framework::EndEvent();

        m_RenderTexture->UnsetActiveTarget();
    }

    Texture& RenderManager::GetRenderTexture()
    {
        return *m_RenderTexture;
    }

    void RenderManager::Resize(u32 width, u32 height)
    {
        m_RenderTexture->Resize(m_Window.GetSize());
    }
}  // namespace fw