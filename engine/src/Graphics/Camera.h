#pragma once

#include "Core/Math/Mat4.h"

namespace fw
{
	class Camera
	{
	public:
		Camera(const Mat4f& projection)
		    : m_Projection(projection)
		{ }

		const Mat4f& GetProjection() const
		{
			return m_Projection;
		}

	protected:
		Mat4f m_Projection;
	};
}  // namespace fw