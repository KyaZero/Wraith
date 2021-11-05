#pragma once

#include <Scene/Entity.h>

#include "Panel.h"

namespace Wraith
{
    class PropertiesPanel : public Panel
    {
    public:
        virtual ~PropertiesPanel() = default;

        void OnUIRender() override;
        const char* GetName() const override { return "Properties"; }

        void SetSelectedEntity(Entity entity) { m_SelectedEntity = entity; }

    private:
        void DrawComponents();

        Entity m_SelectedEntity;
    };
}  // namespace Wraith