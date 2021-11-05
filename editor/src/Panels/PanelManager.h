#pragma once

#include "Panel.h"
#include "PanelGroup.h"

namespace Wraith
{
    class PanelManager : public PanelGroup
    {
    public:
        PanelManager()
            : PanelGroup("View")
        { }
        ~PanelManager() = default;

        void RenderMenus();
        void RenderWindows();

    private:
    };

}  // namespace Wraith