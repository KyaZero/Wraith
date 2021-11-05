#include "OrthographicCameraController.h"

#include "Window/Window.h"

namespace Wraith
{
    OrthographicCameraController::OrthographicCameraController()
        : m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel)
        , m_Rotation(false)
        , m_Input()
    { }

    OrthographicCameraController::OrthographicCameraController(f32 width, f32 height, bool rotation)
        : m_AspectRatio(width / height)
        , m_Camera(0, width, height, 0)
        , m_Rotation(rotation)
        , m_Bounds(width, height)
    {
        SetProjection(width, height);
        Window::RegisterResizeCallback(this, [&](auto w, auto h) { Resize(w, h); });
    }

    OrthographicCameraController::~OrthographicCameraController() { Window::UnregisterResizeCallback(this); }

    void OrthographicCameraController::Init(f32 width, f32 height, bool rotation)
    {
        m_Bounds = { width, height };
        m_AspectRatio = width / height;
        SetProjection(width, height);
        m_Rotation = rotation;
    }

    void OrthographicCameraController::Update(f32 dt)
    {
        if (m_Input.IsDown(Key::A))
        {
            m_CameraPosition.x -= cos(Radians(m_CameraRotation)) * m_CameraTranslationSpeed * dt;
            m_CameraPosition.y -= sin(Radians(m_CameraRotation)) * m_CameraTranslationSpeed * dt;
        }
        else if (m_Input.IsDown(Key::D))
        {
            m_CameraPosition.x += cos(Radians(m_CameraRotation)) * m_CameraTranslationSpeed * dt;
            m_CameraPosition.y += sin(Radians(m_CameraRotation)) * m_CameraTranslationSpeed * dt;
        }
        if (m_Input.IsDown(Key::W))
        {
            m_CameraPosition.x -= -sin(Radians(m_CameraRotation)) * m_CameraTranslationSpeed * dt;
            m_CameraPosition.y -= cos(Radians(m_CameraRotation)) * m_CameraTranslationSpeed * dt;
        }
        else if (m_Input.IsDown(Key::S))
        {
            m_CameraPosition.x += -sin(Radians(m_CameraRotation)) * m_CameraTranslationSpeed * dt;
            m_CameraPosition.y += cos(Radians(m_CameraRotation)) * m_CameraTranslationSpeed * dt;
        }

        if (m_Rotation)
        {
            if (m_Input.IsDown(Key::Q))
                m_CameraRotation -= m_CameraRotationSpeed * dt;
            if (m_Input.IsDown(Key::E))
                m_CameraRotation += m_CameraRotationSpeed * dt;

            if (m_CameraRotation > 180.0f)
                m_CameraRotation -= 360.0f;
            else if (m_CameraRotation <= -180.0f)
                m_CameraRotation += 360.0f;

            m_Camera.SetRotation(m_CameraRotation);
        }

        if (m_Input.IsScrolling())
        {
            auto scroll = m_Input.GetScrollState();
            m_ZoomLevel -= scroll.y_offset * 0.0025f;
            m_ZoomLevel = Max(m_ZoomLevel, 0.0025f);
            SetProjection(m_Bounds.x, m_Bounds.y);
        }

        m_Camera.SetPosition(m_CameraPosition);

        m_CameraTranslationSpeed = m_CameraTranslationBaseSpeed * m_ZoomLevel;
    }

    void OrthographicCameraController::Resize(f32 width, f32 height)
    {
        m_AspectRatio = width / height;
        m_Bounds = { width, height };
        SetProjection(width, height);
    }

    void OrthographicCameraController::SetProjection(f32 width, f32 height)
    {
        m_Camera.SetProjection(-width * m_ZoomLevel, width * m_ZoomLevel, height * m_ZoomLevel, -height * m_ZoomLevel);
    }
}  // namespace Wraith