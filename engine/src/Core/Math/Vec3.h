#pragma once

#include <cmath>

#include <dubu_serialize/dubu_serialize.hpp>

#pragma warning(disable : 4201)

namespace fw
{
    template <typename T>
    class Vec3
    {
    public:
        Vec3()
            : x(0)
            , y(0)
            , z(0)
        { }

        Vec3(T val)
            : x(val)
            , y(val)
            , z(val)
        { }

        Vec3(T _x, T _y, T _z)
            : x(_x)
            , y(_y)
            , z(_z)
        { }

        Vec3(const Vec3& other)
            : x(other.x)
            , y(other.y)
            , z(other.z)
        { }

        Vec3(Vec3&& other)
            : x(other.x)
            , y(other.y)
            , z(other.z)
        { }

        ~Vec3() = default;

        inline Vec3& operator=(const Vec3& other)
        {
            x = other.x;
            y = other.y;
            z = other.z;
            return *this;
        }

        inline Vec3& operator=(Vec3&& other)
        {
            x = other.x;
            y = other.y;
            z = other.z;
            return *this;
        }

        inline T Dot(const Vec3& other) const
        {
            return x * other.x + y * other.y + z * other.z;
        }

        inline Vec3 Cross(const Vec3& other) const
        {
            return { y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x };
        }

        inline T LengthSqr() const
        {
            return Dot(*this);
        }

        inline T Length() const
        {
            return (T)sqrt((T)LengthSqr());
        }

        inline Vec3 GetNormalized() const
        {
            if (T l = Length(); l != 0)
            {
                T invlen = 1 / l;
                return { x * invlen, y * invlen, z * invlen };
            }
            return { 0, 0, 0 };
        }

        inline void Normalize()
        {
            *this = GetNormalized();
        }

        inline bool operator==(const Vec3& other) const
        {
            return x == other.x && y == other.y && z == other.z;
        }

        inline bool operator!=(const Vec3& other) const
        {
            return x != other.x && y != other.y && z != other.z;
        }

        inline Vec3 operator+(const Vec3& other) const
        {
            return { x + other.x, y + other.y, z + other.z };
        }
        inline Vec3 operator-(const Vec3& other) const
        {
            return { x - other.x, y - other.y, z - other.z };
        }
        inline Vec3 operator*(const Vec3& other) const
        {
            return { x * other.x, y * other.y, z * other.z };
        }
        inline Vec3 operator/(const Vec3& other) const
        {
            return { x / other.x, y / other.y, z / other.z };
        }
        inline Vec3 operator*(const T& scalar) const
        {
            return { x * scalar, y * scalar, z * scalar };
        }
        inline Vec3 operator/(const T& scalar) const
        {
            return { x / scalar, y / scalar, z / scalar };
        }

        inline void operator+=(const Vec3& other)
        {
            *this = *this + other;
        }
        inline void operator-=(const Vec3& other)
        {
            *this = *this - other;
        }
        inline void operator*=(const Vec3& other)
        {
            *this = *this * other;
        }
        inline void operator/=(const Vec3& other)
        {
            *this = *this / other;
        }
        inline void operator*=(const T& scalar)
        {
            *this = *this * scalar;
        }
        inline void operator/=(const T& scalar)
        {
            *this = *this / scalar;
        }

        void Serialize(dubu::serialize::ReadBuffer& buffer)
        {
            buffer >> x >> y >> z;
        }
        void Serialize(dubu::serialize::WriteBuffer& buffer) const
        {
            buffer << x << y << z;
        }

        union
        {
            struct
            {
                T x, y, z;
            };
            struct
            {
                T r, g, b;
            };
        };
    };

    using Vec3f = Vec3<f32>;
    using Vec3i = Vec3<i32>;
    using Vec3u = Vec3<u32>;
}  // namespace fw