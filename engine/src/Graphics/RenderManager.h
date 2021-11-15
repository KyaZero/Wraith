#pragma once

#include "Core/Constants.h"
#include "ForwardRenderer.h"
#include "RenderCommand.h"
#include "SpriteRenderer.h"
#include "TextRenderer.h"

namespace Wraith
{
    class RenderManager
    {
    public:
        RenderManager(Window& window);
        ~RenderManager();

        bool Init();

        void Submit(const RenderCommand& command);
        void Render();

        Texture& GetRenderTexture();
        void Resize(u32 width, u32 height);

        void Flip();

    private:
        std::unique_ptr<Texture> m_RenderTexture;
        Window& m_Window;
        SpriteRenderer m_SpriteRenderer;
        ForwardRenderer m_ForwardRenderer;
        TextRenderer m_TextRenderer;
        std::vector<RenderCommand> m_RenderCommands[FRAME_COUNT];
    };
}  // namespace Wraith