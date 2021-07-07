#pragma once

#include "Core/Math/Mat4.h"

namespace fw
{
    class Camera
    {
    public:
        Camera()
        { }
        Camera(const Mat4f& projection)
            : m_Projection(projection)
        { }

        const Mat4f& GetProjection() const
        {
            return m_Projection;
        }

        void Serialize(dubu::serialize::ReadBuffer& buffer)
        {
            buffer >> m_Projection;
        }
        void Serialize(dubu::serialize::WriteBuffer& buffer) const
        {
            buffer << m_Projection;
        }

    protected:
        Mat4f m_Projection;
    };
}  // namespace fw