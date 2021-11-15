#pragma once

#include "Panels/Panel.h"

namespace Wraith
{
    class ThreadSchedulerPanel : public Panel
    {
    public:
        ThreadSchedulerPanel() = default;
        virtual ~ThreadSchedulerPanel() = default;

        void OnUIRender() override;
        const char* GetName() const override { return "Thread Scheduler"; }
    };

}  // namespace Wraith