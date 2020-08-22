#include "OrthographicCameraController.h"
#include "Window/Input.h"

namespace fw
{
	OrthographicCameraController::OrthographicCameraController() : m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(false)
	{
	}

	OrthographicCameraController::OrthographicCameraController(f32 width, f32 height, bool rotation) : 
		m_AspectRatio(width/height), 
		m_Camera(0, width, height, 0),
		m_Rotation(rotation),
		m_Bounds(width, height)
	{
	}

	OrthographicCameraController::~OrthographicCameraController()
	{
	}

	void OrthographicCameraController::Init(f32 width, f32 height, bool rotation)
	{
		m_Bounds = { width, height };
		m_AspectRatio = width / height;
		SetProjection(width, height);
		m_Rotation = rotation;
	}

	void OrthographicCameraController::Update(f32 dt)
	{
		if (Input::IsKeyDown(Key::A))
		{
			m_CameraPosition.x -= cos(Radians(m_CameraRotation)) * m_CameraTranslationSpeed * dt;
			m_CameraPosition.y -= sin(Radians(m_CameraRotation)) * m_CameraTranslationSpeed * dt;
		}
		else if (Input::IsKeyDown(Key::D))
		{
			m_CameraPosition.x += cos(Radians(m_CameraRotation)) * m_CameraTranslationSpeed * dt;
			m_CameraPosition.y += sin(Radians(m_CameraRotation)) * m_CameraTranslationSpeed * dt;
		}

		if (Input::IsKeyDown(Key::W))
		{
			m_CameraPosition.x -= -sin(Radians(m_CameraRotation)) * m_CameraTranslationSpeed * dt;
			m_CameraPosition.y -= cos(Radians(m_CameraRotation)) * m_CameraTranslationSpeed * dt;
		}
		else if (Input::IsKeyDown(Key::S))
		{
			m_CameraPosition.x += -sin(Radians(m_CameraRotation)) * m_CameraTranslationSpeed * dt;
			m_CameraPosition.y += cos(Radians(m_CameraRotation)) * m_CameraTranslationSpeed * dt;
		}

		if (m_Rotation)
		{
			if (Input::IsKeyDown(Key::Q))
				m_CameraRotation -= m_CameraRotationSpeed * dt;
			if (Input::IsKeyDown(Key::E))
				m_CameraRotation += m_CameraRotationSpeed * dt;

			if (m_CameraRotation > 180.0f)
				m_CameraRotation -= 360.0f;
			else if (m_CameraRotation <= -180.0f)
				m_CameraRotation += 360.0f;

			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);

		m_CameraTranslationSpeed = m_CameraTranslationBaseSpeed * m_ZoomLevel;
	}

	void OrthographicCameraController::OnEvent(const Event& e)
	{
		if (e.type == Event::Resized)
			Resize(e.size.width, e.size.height);

		if (e.type == Event::MouseWheelScrolled)
			OnMouseScrolled(e);
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

	void OrthographicCameraController::OnMouseScrolled(const Event& e)
	{
		m_ZoomLevel -= e.mousewheel_scroll.delta * 0.25f;
		m_ZoomLevel = Max(m_ZoomLevel, 0.25f);
		SetProjection(m_Bounds.x, m_Bounds.y);
	}
}