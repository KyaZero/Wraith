#pragma once

#include "Panel.h"

namespace Wraith
{
    class PanelGroup
    {
    public:
        PanelGroup(std::string_view name)
            : m_Name(name)
        { }
        virtual ~PanelGroup() = default;

        void RenderWindows();
        void RenderMenus();

        template <class T = PanelGroup, class... Args>
        std::enable_if_t<std::is_base_of_v<PanelGroup, T>, T*> CreateGroup(Args&&... args)
        {
            m_PanelGroups.push_back(std::move(std::make_unique<T>(std::forward<Args>(args)...)));
            return reinterpret_cast<T*>(m_PanelGroups.back().get());
        }
        template <class T, class... Args>
        std::enable_if_t<std::is_base_of_v<Panel, T>, T*> CreatePanel(Args&&... args)
        {
            m_Panels.push_back(std::move(std::make_unique<T>(std::forward<Args>(args)...)));
            return reinterpret_cast<T*>(m_Panels.back().get());
        }

    private:
        std::string m_Name;
        std::vector<std::unique_ptr<Panel>> m_Panels;
        std::vector<std::unique_ptr<PanelGroup>> m_PanelGroups;
    };
}  // namespace Wraith