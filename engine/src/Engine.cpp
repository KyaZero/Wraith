#include "Engine.h"

#include "Core/Common.h"
#include "Core/Filewatcher.h"
#include "Core/Logger.h"
#include "Graphics/ContentManager.h"
#include "Graphics/TextureManager.h"

namespace fw
{
    Engine::Engine(Window& window)
        : m_Window(window)
        , m_Framework(window)
        , m_RenderManager(window)
    {
        Logger::Create();
        Filewatcher::Create();
        ContentManager::Create();
        TextureManager::Create();
    }

    Engine::~Engine()
    {
        TextureManager::Destroy();
        ContentManager::Destroy();
        Filewatcher::Destroy();
        Logger::Destroy();
    }

    bool Engine::Init()
    {
        if (!m_Framework.Init())
            return false;

        if (!m_RenderManager.Init())
            return false;

        return true;
    }

    void Engine::BeginFrame()
    {
        m_Framework.BeginFrame({ 0.2f, 0.2f, 0.2f, 1 });
    }

    void Engine::EndFrame()
    {
        m_Framework.EndFrame();
    }

    void Engine::Update(f32 dt, f32 total_time)
    {
        m_RenderManager.Render(dt, total_time);
        m_Framework.SetBackbufferAsActiveTarget();
        Filewatcher::Get()->FlushChanges();
        Input::FlushState();

        // Calculate average fps and display it on the window title
        {
            static auto original_title = m_Window.GetTitle();

            constexpr float smoothness = 0.99f;
            m_Fps = (m_Fps * smoothness) + ((1.0f / dt) * (1.0 - smoothness));

            m_FpsUpdateTimer -= dt;
            if (m_FpsUpdateTimer <= std::numeric_limits<float>::epsilon())
            {
                m_Window.SetTitle(original_title + " - FPS: " + std::to_string((u32)(m_Fps)));
                m_FpsUpdateTimer += 1.f / FpsUpdatesPerSecond;
            }
        }
    }
}  // namespace fw