#include "Panel.h"

#include <imgui/imgui.h>

namespace Wraith
{
    bool Panel::Begin(int flags)
    {
        mShouldCallEnd = mIsVisible;
        if (!mIsVisible)
            return false;

        bool visible = ImGui::Begin(GetName(), &mIsVisible, flags);

        if (visible)
        {
            mIsFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows);
            mIsHovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows);
        }

        return visible;
    }

    void Panel::End()
    {
        if (mShouldCallEnd)
            ImGui::End();
    }

}  // namespace Wraith