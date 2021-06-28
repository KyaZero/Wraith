#pragma once

#include "Core/Types.h"
#include "RenderCommand.h"
#include "SpriteRenderer.h"

namespace fw
{
    class RenderManager
    {
    public:
        RenderManager(Window& window);
        ~RenderManager();

        bool Init();

        void Submit(const RenderCommand& command);
        void Render(f32 dt, f32 total_time);

        Texture& GetRenderTexture();
        void Resize(u32 width, u32 height);

    private:
        std::unique_ptr<Texture> m_RenderTexture;
        Window& m_Window;
        SpriteRenderer m_SpriteRenderer;
        std::vector<RenderCommand> m_RenderCommands;
    };
}  // namespace fw