#pragma once

#include "BaseRenderer.h"
#include "Buffer.h"
#include "Core/Constants.h"
#include "Core/Math/Mat4.h"
#include "OrthographicCamera.h"
#include "RenderCommand.h"
#include "Sampler.h"
#include "Shader.h"
#include "Texture.h"

namespace Wraith
{
    class SpriteRenderer : public BaseRenderer<SpriteCommand>
    {
        // I found 1024 instances to be the best in terms of performance and drawcalls
        constexpr static u32 MAX_INSTANCES = 1024;

    public:
        SpriteRenderer(Window& window);
        virtual ~SpriteRenderer();

        bool Init() override;
        void Render();

        void SetCamera(const SetCameraCommand& command);

    private:
        Window& m_Window;

        struct RenderCamera
        {
            Mat4f projection;
            Mat4f view;
        };

        std::unique_ptr<RenderCamera> m_CurrentCamera;

        Shader m_SpriteShader;

        Buffer m_IndexBuffer;
        Buffer m_VertexBuffer;
        Buffer m_InstanceBuffer;

        struct ConstantBufferData
        {
            Mat4f view_projection;
            Mat4f projection;
            Vec2f resolution;
        } m_ConstantBufferData;
        Buffer m_ConstantBuffer;

        struct InstanceData
        {
            Vec4f color;
            Vec2f position;
            Vec2f offset;
            Vec2f scale;
            Vec2f size;
            f32 rotation;
            i32 world_space;
        };

        void UpdateConstantBuffer();

        Sampler m_Sampler;
    };
}  // namespace Wraith