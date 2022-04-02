#pragma once

#include "Core/Timer.h"
#include "Graphics/RenderManager.h"
#include "Graphics/VkFramework.h"
#include "Scene/Scene.h"
#include "UI/ImGuiLayer.h"

namespace Wraith
{
    class Engine
    {
    public:
        Engine(Window& window);
        ~Engine();

        bool Init(
            std::function<void()> ui_render_callback = []() {}, std::function<void(f32)> update_callback = [](auto) {});
        void Update();

        void Render();

        void BeginFrame();
        void EndFrame();

        RenderManager* GetRenderer() { return &m_RenderManager; }
        Window& GetWindow() { return m_Window; }
        VkFramework& GetFramework() { return m_Framework; }
        ImguiLayer& GetImguiLayer() { return *m_ImguiLayer; }

    private:
        Window& m_Window;
        VkFramework m_Framework;
        RenderManager m_RenderManager;
        Scene m_Scene;

        std::unique_ptr<ImguiLayer> m_ImguiLayer;
        std::function<void()> m_UIRenderCallback;
        std::function<void(f32)> m_UpdateCallback;

        Timer m_Timer;

        f32 m_Fps = 0.0f;
        f32 m_FpsUpdateTimer = 0.0f;
        constexpr static f32 FpsUpdatesPerSecond = 30.0f;
    };
}  // namespace Wraith