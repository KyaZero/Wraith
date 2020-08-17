#include "OrthographicCamera.h"

namespace fw
{
	OrthographicCamera::OrthographicCamera(f32 width, f32 height)
		: m_ProjectionMatrix(Mat4f::CreateOrthographicProjection(0, width, height, 0, -1.0f, 1.0f))
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	OrthographicCamera::~OrthographicCamera()
	{
	}

	void OrthographicCamera::SetProjection(f32 width, f32 height)
	{
		m_ProjectionMatrix = Mat4f::CreateOrthographicProjection(0, width, height, 0, -1.0f, 1.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		Mat4f transform = Mat4f::Scale(Mat4f(), Vec3f(m_Scale, m_Scale, 1)) * Mat4f::Translate(Mat4f(), m_Position) * Mat4f::Rotate(Mat4f(), Radians(m_Rotation), Vec3f(0, 0, 1)) ;

		m_ViewMatrix = m_ViewMatrix.Inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}