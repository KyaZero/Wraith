#include "PanelSettings.h"

#include <deque>

#include "Core/Constants.h"

namespace Wraith
{
    std::unordered_set<std::string> SettingsHandler::s_VisiblePanels;

    void SettingsHandler::Register(PanelGroup* root)
    {
        ImGuiSettingsHandler handler;
        handler.TypeName = PROJECT_NAME;
        handler.TypeHash = ImHashStr(handler.TypeName);
        handler.ClearAllFn = ClearAll;
        handler.ReadInitFn = ClearAll;
        handler.ReadOpenFn = ReadOpen;
        handler.ReadLineFn = ReadLine;
        handler.ApplyAllFn = ApplyAll;
        handler.WriteAllFn = WriteAll;
        handler.UserData = reinterpret_cast<void*>(root);
        auto g = ImGui::GetCurrentContext();
        g->SettingsHandlers.push_back(handler);
    }

    void SettingsHandler::ClearAll(ImGuiContext*, ImGuiSettingsHandler*) { s_VisiblePanels.clear(); }
    void SettingsHandler::ApplyAll(ImGuiContext*, ImGuiSettingsHandler* handler)
    {
        auto root = reinterpret_cast<PanelGroup*>(handler->UserData);
        Traverse(root, [&](Panel* p) { p->SetVisibility(s_VisiblePanels.count(p->GetName()) > 0); });
    }
    void* SettingsHandler::ReadOpen(ImGuiContext*, ImGuiSettingsHandler*, const char* name)
    {
        if (name != std::string_view("Data"))
            return nullptr;
        return reinterpret_cast<void*>(1);
    }
    void SettingsHandler::ReadLine(ImGuiContext*, ImGuiSettingsHandler*, void* entry, const char* line) { s_VisiblePanels.insert(line); }
    void SettingsHandler::WriteAll(ImGuiContext* imgui_ctx, ImGuiSettingsHandler* handler, ImGuiTextBuffer* buf)
    {
        buf->appendf("[%s][Data]\n", handler->TypeName);

        auto root = reinterpret_cast<PanelGroup*>(handler->UserData);
        Traverse(root, [&](Panel* p) {
            if (p->IsVisible())
                buf->appendf("%s\n", p->GetName());
        });
    }
    void SettingsHandler::Traverse(PanelGroup* root, std::function<void(Panel*)> callback)
    {
        auto groups = std::deque<PanelGroup*>{ root };
        while (!groups.empty())
        {
            auto group = groups.front();
            groups.pop_front();
            for (const auto& p : group->GetPanels())
            {
                callback(p.get());
            }
            for (const auto& g : group->GetGroups())
            {
                groups.push_back(g.get());
            }
        }
    }
}  // namespace Wraith