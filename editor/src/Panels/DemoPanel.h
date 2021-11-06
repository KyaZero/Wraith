#pragma once

#include "Panel.h"

namespace Wraith
{
    class DemoPanel : public Panel
    {
        // Inherited via Panel
        virtual void OnUIRender() override;
        virtual const char* GetName() const override { return "ImGui Demo Window"; }
    };
}  // namespace Wraith