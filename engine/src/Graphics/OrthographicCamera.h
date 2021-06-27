#pragma once
#include "Core/Math/Mat4.h"
#include "Core/Math/Vec2.h"
#include "Core/Types.h"
#include "Camera.h"

namespace fw
{
    class OrthographicCamera : public Camera
    {
    public:
        OrthographicCamera(f32 left, f32 right, f32 bottom, f32 top);
        ~OrthographicCamera();

        void SetProjection(f32 left, f32 right, f32 bottom, f32 top);

        const Vec3f& GetPosition() const { return m_Position; }
        void SetPosition(const Vec3f& position) { m_Position = position; RecalculateViewMatrix(); }

        f32 GetRotation() { return m_Rotation; }
        void SetRotation(f32 rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }

        f32 GetScale() { return m_Scale; }
        void SetScale(f32 scale) { m_Scale = scale; RecalculateViewMatrix(); }

        const Mat4f& GetView() const { return m_ViewMatrix; }
        const Mat4f& GetViewProjection() const { return m_ViewProjectionMatrix; }

        const Vec2f Unproject(const Vec2f& pos) const;

    private:
        void RecalculateViewMatrix();

        Mat4f m_ViewProjectionMatrix;
        Mat4f m_ViewMatrix;

        Vec3f m_Position = { 0.0f, 0.0f, 0.0f };
        f32 m_Rotation = 0.0f;
        f32 m_Scale = 1.0f;
    };
}