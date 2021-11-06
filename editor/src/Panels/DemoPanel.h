#pragma once

#include "Panel.h"

namespace Wraith
{
    class DemoPanel : public Panel
    {
    public:
        void OnUIRender() override;
        const char* GetName() const override { return "ImGui Demo Window"; }
    };
}  // namespace Wraith