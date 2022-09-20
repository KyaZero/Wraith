#pragma once
#include "BaseRenderer.h"
#include "Sampler.h"

namespace Wraith
{
    class ForwardRenderer : public BaseRenderer<ModelCommand>
    {
    public:
        ForwardRenderer();
        ~ForwardRenderer();

        bool Init() override;

        void SetCamera(const SetCameraCommand& command);

        void Render();

    private:
        struct FrameBuffer
        {
            Mat4f projection;
            Mat4f view;
        } m_FrameBufferData;

        // Buffer m_FrameBuffer;

        struct ObjectBuffer
        {
            Mat4f transform;
        } m_ObjectBufferData;

        // Buffer m_ObjectBuffer;

        Sampler m_Sampler;

        RenderCamera m_CurrentCamera;
    };
}  // namespace Wraith