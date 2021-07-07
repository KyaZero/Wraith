#include "Engine.h"

#include "Core/Common.h"
#include "Core/Filewatcher.h"
#include "Core/Logger.h"
#include "Graphics/ContentManager.h"
#include "Graphics/TextureManager.h"
#include "Threading/ThreadScheduler.h"
#include "UI/ImGuiLayer.h"

namespace fw
{
    Engine::Engine(Window& window)
        : m_Window(window)
        , m_Framework(window)
        , m_RenderManager(window)
    {
        Logger::Create();
        Logger::SetLevel(Logger::Level::Info);
        Filewatcher::Create();
        ContentManager::Create();
        TextureManager::Create();
        ThreadScheduler::Create();

        ThreadScheduler::Get()->AddPersistentJob(std::make_shared<PersistentJob>("Update Frame", [&](){
            m_UpdateCallback(m_Timer.GetDeltaTime()); 
        }));

        ThreadScheduler::Get()->AddPersistentJob(std::make_shared<PersistentJob>("Render Frame", [&]() {
            Render(); 
        }));
    }

    Engine::~Engine()
    {
        ThreadScheduler::Destroy();
        TextureManager::Destroy();
        ContentManager::Destroy();
        Filewatcher::Destroy();
        Logger::Destroy();
    }

    bool Engine::Init(std::function<void()> ui_render_callback, std::function<void(f32)> update_callback)
    {
        if (!m_Framework.Init())
            return false;

        if (!m_RenderManager.Init())
            return false;

        m_ImguiLayer = std::make_unique<ImguiLayer>(m_Framework, m_Window);

        m_UIRenderCallback = ui_render_callback;
        m_UpdateCallback = update_callback;

        return true;
    }

    void Engine::BeginFrame()
    {
        m_Framework.BeginFrame({ 0.2f, 0.2f, 0.2f, 1 });
    }

    void Engine::EndFrame()
    {
        m_Framework.EndFrame();
        m_RenderManager.Flip();
    }

    void Engine::Update()
    {
        m_Timer.Update();

        ThreadScheduler::Get()->Start();
        ThreadScheduler::Get()->Sync();

        // Render ImGui
        m_ImguiLayer->Begin();
        m_UIRenderCallback();
        ThreadScheduler::Get()->RenderDebugInfo();
        m_ImguiLayer->End();

        Filewatcher::Get()->FlushChanges();
        Input::FlushState();

        // Calculate average fps and display it on the window title
        {
            static auto original_title = m_Window.GetTitle();

            constexpr float smoothness = 0.99f;
            m_Fps = (m_Fps * smoothness) + ((1.0f / m_Timer.GetDeltaTime()) * (1.0 - smoothness));

            m_FpsUpdateTimer -= m_Timer.GetDeltaTime();
            if (m_FpsUpdateTimer <= std::numeric_limits<float>::epsilon())
            {
                m_Window.SetTitle(original_title + " - FPS: " + std::to_string((u32)(m_Fps)));
                m_FpsUpdateTimer += 1.f / FpsUpdatesPerSecond;
            }
        }
    }

    void Engine::Render()
    {
        m_RenderManager.Render();
        m_Framework.SetBackbufferAsActiveTarget();
    }

}  // namespace fw