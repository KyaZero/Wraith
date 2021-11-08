#pragma once
#include "BaseRenderer.h"

namespace Wraith
{
    class ForwardRenderer : BaseRenderer<StaticMeshCommand>
    {
    public:
        ForwardRenderer();
        ~ForwardRenderer();

        bool Init() override;

        void SetCamera(const SetCameraCommand& camera);

        void Render();

    private:
    };
}  // namespace Wraith