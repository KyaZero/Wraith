#pragma once

#include "Graphics/Camera.h"
#include "Input/Input.h"

namespace Wraith
{
    enum class CameraState
    {
        FreeCam,
        ArcBall,
        Idle
    };

    class EditorCamera : public Camera
    {
    public:
        EditorCamera() = default;
        EditorCamera(f32 fov, f32 aspect_ratio, f32 near_clip, f32 far_clip);

        void Update(f32 dt, bool accept_input);

        inline void SetViewportSize(f32 width, f32 height)
        {
            m_ViewportSize.x = width;
            m_ViewportSize.y = height;
            UpdateProjection();
        }

        void SetTarget(Vec3f target, f32 scale_magnitude);

        const Mat4f& GetViewMatrix() const { return m_ViewMatrix; }
        Mat4f GetViewProjection() const { return m_Projection * m_ViewMatrix; }

        Vec3f GetForward() const { return m_Orientation.GetForwardVector(); }
        Vec3f GetRight() const { return m_Orientation.GetRightVector(); }
        Vec3f GetUp() const { return m_Orientation.GetUpVector(); }

        CameraState GetCameraState() const;

    private:
        void UpdateArcball(f32 dt, Vec2f mouse_delta);
        void UpdateFreeCam(f32 dt, Vec2f mouse_delta);

        void UpdateProjection();
        void UpdateView();

        Input m_Input;

        // Arcball variables
        Vec3f m_Eye;
        Vec3f m_TargetLookAt;
        Vec3f m_CurrentLookAt;
        Vec3f m_Up;

        f32 m_CurrentDistance = 20.0f;
        f32 m_TargetDistance = 20.0f;

        // Freecam Variables
        Quatf m_Orientation;
        f32 m_Pitch;
        f32 m_Yaw;

        Mat4f m_ViewMatrix;

        Vec2f m_ViewportSize;
        Vec2f m_LastMousePos;

        f32 m_Fov = 45.0f;
        f32 m_AspectRatio = (16.0f / 9.0f);
        f32 m_NearClip = 0.1f;
        f32 m_FarClip = 1000.0f;

        f32 m_Speed = 3.0f;
        f32 m_SpeedModifier = 1.0f;

        CameraState m_CurrentState = CameraState::Idle;
    };
}  // namespace Wraith