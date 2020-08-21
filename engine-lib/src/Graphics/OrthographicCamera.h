#pragma once
#include "Core/Math/Mat4.h"
#include "Core/Math/Vec3.h"
#include "Core/Types.h"

namespace fw
{
    class OrthographicCamera
    {
    public:
        OrthographicCamera(f32 width, f32 height);
        ~OrthographicCamera();

        void SetProjection(f32 width, f32 height);

        const Vec3f& GetPosition() const { return m_Position; }
        void SetPosition(const Vec3f& position) { m_Position = position; RecalculateViewMatrix(); }

        f32 GetRotation() { return m_Rotation; }
        void SetRotation(f32 rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }

        f32 GetScale() { return m_Scale; }
        void SetScale(f32 scale) { m_Scale = scale; RecalculateViewMatrix(); }

        const Mat4f& GetProjectionMatrix() const { return m_ProjectionMatrix; }
        const Mat4f& GetViewMatrix() const { return m_ViewMatrix; }
        const Mat4f& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

        const Vec2f& GetBounds() const;
        const Vec2f Unproject(const Vec2f& pos) const;

    private:
        void RecalculateViewMatrix();

        Mat4f m_ProjectionMatrix;
        Mat4f m_ViewMatrix;
        Mat4f m_ViewProjectionMatrix;

        Vec3f m_Position = { 0.0f, 0.0f, 0.0f };
        Vec2f m_Bounds = { 0, 0 };
        f32 m_Rotation = 0.0f;
        f32 m_Scale = 1.0f;
    };
}