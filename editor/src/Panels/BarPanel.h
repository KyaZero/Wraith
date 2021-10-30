#pragma once

#include <functional>

#include "Panel.h"

namespace Wraith
{
    class BarPanel : public Panel
    {
        using Handler = std::function<void()>;

    public:
        BarPanel(Handler on_begin_play, Handler on_end_play)
            : m_OnBeginPlay(on_begin_play)
            , m_OnEndPlay(on_end_play)
        { }
        virtual ~BarPanel() = default;

        void OnUIRender() override;
        const char* GetName() const override
        {
            return "Bar";
        }

    private:
        bool m_IsPlay = true;
        Handler m_OnBeginPlay;
        Handler m_OnEndPlay;
    };
}  // namespace Wraith