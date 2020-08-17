#pragma once
#include "Core/Types.h"
#include "Window/Event.h"
#include "OrthographicCamera.h"

namespace fw
{
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController();
		OrthographicCameraController(f32 width, f32 height, bool rotation = false);
		~OrthographicCameraController();

		void Init(f32 width, f32 height, bool rotation = false);
		void Update(f32 dt);
		void OnEvent(const Event& e);
		void Resize(f32 width, f32 height);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

		f32 GetZoomLevel() const { return m_ZoomLevel; }
		void SetZoomLevel(f32 level) { m_ZoomLevel = level; }

	private:
		void OnMouseScrolled(const Event& e);

		f32 m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;

		bool m_Rotation;

		Vec3f m_CameraPosition = { 0, 0, 0 };
		f32 m_CameraRotation = 0.0f;
		f32 m_CameraScaleSpeed = 2.0f;
		f32 m_CameraTranslationSpeed = 5.0f;
		f32 m_CameraRotationSpeed = 180.0f;
	};
}