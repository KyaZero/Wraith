#pragma once

#include "Core/Constants.h"
#include "Framework.h"
#include "RenderCommand.h"
#include "Window/Window.h"

namespace Wraith
{
    /*
        Basic interface class for a double-buffered Renderer
    */
    template <typename Command>
    class BaseRenderer
    {
    public:
        BaseRenderer() { }

        virtual ~BaseRenderer() { }

        virtual void Submit(const Command& command) { GetCurrentCommands().push_back(command); }

        virtual bool Init() { return true; }

        virtual void Flip()
        {
            std::swap(m_Commands[CURRENT_FRAME], m_Commands[NEXT_FRAME]);
            m_Commands[NEXT_FRAME].clear();
        }

    protected:
        std::vector<Command>& GetCurrentCommands() { return m_Commands[CURRENT_FRAME]; }

    private:
        std::vector<Command> m_Commands[FRAME_COUNT];
    };
}  // namespace Wraith