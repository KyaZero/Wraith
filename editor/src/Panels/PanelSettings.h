#pragma once

#include <unordered_set>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include "PanelGroup.h"

namespace Wraith
{
    class SettingsHandler
    {
    public:
        static void Register(PanelGroup* root);

    private:
        static void ClearAll(ImGuiContext*, ImGuiSettingsHandler*);
        static void ApplyAll(ImGuiContext*, ImGuiSettingsHandler*);
        static void* ReadOpen(ImGuiContext*, ImGuiSettingsHandler*, const char* name);
        static void ReadLine(ImGuiContext*, ImGuiSettingsHandler*, void* entry, const char* line);
        static void WriteAll(ImGuiContext* imgui_ctx, ImGuiSettingsHandler* handler, ImGuiTextBuffer* buf);

        static void Traverse(PanelGroup* root, std::function<void(Panel*)> callback);

        static std::unordered_set<std::string> s_VisiblePanels;
    };
}  // namespace Wraith
