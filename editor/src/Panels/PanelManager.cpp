#include "PanelManager.h"

#include <imgui/imgui.h>

namespace Wraith
{
    void PanelManager::RenderMenus() { PanelGroup::RenderMenus(); }

    void PanelManager::RenderWindows() { PanelGroup::RenderWindows(); }
}  // namespace Wraith