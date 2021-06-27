#include "OrthographicCamera.h"

namespace fw
{
	OrthographicCamera::OrthographicCamera(f32 left, f32 right, f32 bottom, f32 top)
	    : Camera(m_Projection)
	{
		SetProjection(left, right, bottom, top);
		m_ViewProjectionMatrix = m_ViewMatrix * m_Projection;
	}

	OrthographicCamera::~OrthographicCamera()
	{ }

	void OrthographicCamera::SetProjection(f32 left, f32 right, f32 bottom, f32 top)
	{
		m_Projection = Mat4f::CreateOrthographicProjection(left, right, bottom, top, -1.0f, 1.0f);
		m_ViewProjectionMatrix = m_ViewMatrix * m_Projection;
	}

	const Vec2f OrthographicCamera::Unproject(const Vec2f& pos) const
	{
		auto ret = Vec4f{ pos.x * 2.0f - 1.0f, pos.y * 2.0f - 1.0f, 0, 1 };
		auto mat = Mat4f::Inverse(m_ViewMatrix) * Mat4f::Inverse(m_Projection);
		ret *= mat;
		return ret.xy;
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		Mat4f transform = Mat4f();
		transform = Mat4f::Scale(Mat4f(), Vec3f(m_Scale, m_Scale, 1));
		transform = Mat4f::Rotate(transform, Radians(m_Rotation), Vec3f(0, 0, 1));
		transform = Mat4f::Translate(transform, m_Position);
		m_ViewMatrix = Mat4f::FastInverse(transform);
		m_ViewProjectionMatrix = m_ViewMatrix * m_Projection;
	}
}  // namespace fw