#include "RenderManager.h"

#include "Framework.h"

namespace Wraith
{
    RenderManager::RenderManager(Window& window)
        : m_Window(window)
        , m_SpriteRenderer(window)
    { }

    RenderManager::~RenderManager() { }

    bool RenderManager::Init()
    {
        // if (!m_SpriteRenderer.Init())
        //    return false;

        // if (!m_TextRenderer.Init())
        //    return false;

        // if (!m_ForwardRenderer.Init())
        //    return false;

        // m_RenderTexture = std::make_unique<Texture>();
        // m_RenderTexture->Create(m_Window.GetSize());

        // m_RenderDepth = std::make_unique<Texture>();
        // m_RenderDepth->CreateDepth(m_Window.GetSize());

        Window::RegisterResizeCallback(this, [&](auto w, auto h) { Resize(w, h); });
        return true;
    }

    void RenderManager::Submit(const RenderCommand& command) { m_RenderCommands[NEXT_FRAME].push_back(command); }

    void RenderManager::Render()
    {
        // m_RenderTexture->Clear({ 0.0f, 0.0f, 0.0f, 1.0f });
        // m_RenderDepth->ClearDepth();
        // m_RenderTexture->SetAsActiveTarget(m_RenderDepth.get());

        // for (auto& command : m_RenderCommands[CURRENT_FRAME])
        //{
        //    std::visit(variant_visitor{
        //                   [&](SpriteCommand sprite) { m_SpriteRenderer.Submit(sprite); },
        //                   [&](SetCameraCommand camera) {
        //                       m_SpriteRenderer.SetCamera(camera);
        //                       m_ForwardRenderer.SetCamera(camera);
        //                   },
        //                   [&](TextCommand text) { m_TextRenderer.Submit(text); },
        //                   [&](ModelCommand model) { m_ForwardRenderer.Submit(model); },
        //               },
        //               command);
        //}

        // Framework::BeginEvent("Render Meshes (Forward)");
        // m_ForwardRenderer.Render();
        // Framework::EndEvent();

        // Framework::BeginEvent("Render Sprites");
        // m_SpriteRenderer.Render();
        // Framework::EndEvent();

        //// Unset depth stencil
        // m_RenderTexture->SetAsActiveTarget();

        // Framework::BeginEvent("Render Text");
        // m_TextRenderer.Render(m_RenderTexture->GetSizef());
        // Framework::EndEvent();

        // m_RenderTexture->UnsetActiveTarget();
    }

    Texture& RenderManager::GetRenderTexture() { return *m_RenderTexture; }

    void RenderManager::Resize(u32 width, u32 height)
    {
        //m_RenderTexture->Resize({ width, height });
        //m_RenderDepth->Resize({ width, height });
    }

    void RenderManager::Flip()
    {
        std::swap(m_RenderCommands[CURRENT_FRAME], m_RenderCommands[NEXT_FRAME]);
        m_RenderCommands[NEXT_FRAME].clear();
        m_SpriteRenderer.Flip();
        m_TextRenderer.Flip();
        m_ForwardRenderer.Flip();
    }
}  // namespace Wraith
