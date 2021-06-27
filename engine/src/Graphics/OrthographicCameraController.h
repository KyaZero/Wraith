#pragma once

#include "Core/Types.h"
#include "Input/Input.h"
#include "OrthographicCamera.h"

namespace fw
{
    class OrthographicCameraController
    {
    public:
        OrthographicCameraController();
        OrthographicCameraController(f32 width, f32 height, bool rotation = false);
        ~OrthographicCameraController();

        void Init(f32 width, f32 height, bool rotation = true);
        void Update(f32 dt);
        void Resize(f32 width, f32 height);

        OrthographicCamera* GetCamera()
        {
            return &m_Camera;
        }
        const OrthographicCamera* GetCamera() const
        {
            return &m_Camera;
        }

        f32 GetZoomLevel() const
        {
            return m_ZoomLevel;
        }
        void SetZoomLevel(f32 level)
        {
            m_ZoomLevel = level;
        }

    private:
        void SetProjection(f32 width, f32 height);

        Input m_Input;

        f32 m_AspectRatio = 1.0f;
        f32 m_ZoomLevel = 0.0025f;
        OrthographicCamera m_Camera;
        Vec2f m_Bounds;

        bool m_Rotation;

        Vec3f m_CameraPosition = { 0, 0, 0 };
        f32 m_CameraRotation = 0.0f;
        f32 m_CameraTranslationBaseSpeed = 1000.0f;
        f32 m_CameraTranslationSpeed = 0.0f;
        f32 m_CameraRotationSpeed = 180.0f;
    };
}  // namespace fw