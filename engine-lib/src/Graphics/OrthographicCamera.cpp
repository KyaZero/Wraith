#include "OrthographicCamera.h"

namespace fw
{
	OrthographicCamera::OrthographicCamera(f32 width, f32 height)
		: m_ProjectionMatrix(Mat4f::CreateOrthographicProjection(0, width, height, 0, -1.0f, 1.0f)), m_Bounds(width, height)
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	OrthographicCamera::~OrthographicCamera()
	{
	}

	void OrthographicCamera::SetProjection(f32 width, f32 height)
	{
		m_ProjectionMatrix = Mat4f::CreateOrthographicProjection(0, width, height, 0, -1.0f, 1.0f);
		m_Bounds = Vec2f(width, height);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	const Vec2f& OrthographicCamera::GetBounds() const
	{
		return m_Bounds;
	}

	const Vec2f OrthographicCamera::Unproject(const Vec2f& pos) const
	{
		auto ret = Vec4f{ pos.x * 2.0f - 1.0f, pos.y * 2.0f - 1.0f, 0, 1 };
		auto mat = Mat4f::Inverse(m_ViewMatrix) * Mat4f::Inverse(m_ProjectionMatrix);
		ret *= mat;
		return ret.xy;
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		Mat4f transform = Mat4f::Scale(Mat4f(), Vec3f(m_Scale, m_Scale, 1)) * Mat4f::Translate(Mat4f(), m_Position) * Mat4f::Rotate(Mat4f(), Radians(m_Rotation), Vec3f(0, 0, 1)) ;

		m_ViewMatrix = Mat4f::Inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}