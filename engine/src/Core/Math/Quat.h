#pragma once

#include <algorithm>

#include "Core/Common.h"
#include "Mat4.h"

namespace fw
{
    template <class T>
    class Mat3;

    template <class T>
    class Mat4;

    template <class T>
    class Vec3;

    template <class T>
    class Quat
    {
    public:
        Quat<T>()
        {
            w = static_cast<T>(1);
            x = static_cast<T>(0);
            y = static_cast<T>(0);
            z = static_cast<T>(0);
        }
        Quat<T>(const T& w, const T& x, const T& y, const T& z)
            : w(w)
            , x(x)
            , y(y)
            , z(z)
        { }
        Quat<T>(const T& yaw, const T& pitch, const T& roll)
        {
            T cy = cos(yaw * T(0.5));
            T sy = sin(yaw * T(0.5));
            T cr = cos(roll * T(0.5));
            T sr = sin(roll * T(0.5));
            T cp = cos(pitch * T(0.5));
            T sp = sin(pitch * T(0.5));

            w = cy * cr * cp + sy * sr * sp;
            x = cy * sr * cp - sy * cr * sp;
            y = cy * cr * sp + sy * sr * cp;
            z = sy * cr * cp - cy * sr * sp;
        }

        explicit Quat<T>(const Vec3<T>& yawPitchRoll)
        {
            T cx = cos(yawPitchRoll.x * T(0.5));
            T cy = cos(yawPitchRoll.y * T(0.5));
            T cz = cos(yawPitchRoll.z * T(0.5));
            T sx = sin(yawPitchRoll.x * T(0.5));
            T sy = sin(yawPitchRoll.y * T(0.5));
            T sz = sin(yawPitchRoll.z * T(0.5));

            w = cx * cy * cz + sx * sy * sz;
            x = sx * cy * cz - cx * sy * sz;
            y = cx * sy * cz + sx * cy * sz;
            z = cx * cy * sz - sx * sy * cz;
        }
        Quat<T>(const Vec3<T>& vector, const T angle)
        {
            T halfAngle = angle / T(2);
            w = cos(halfAngle);
            T halfAngleSin = sin(halfAngle);
            x = vector.x * halfAngleSin;
            y = vector.y * halfAngleSin;
            z = vector.z * halfAngleSin;
        }

        template <typename U>
        Quat<T>(const Mat4<U>& matrix)
        {
            w = std::sqrt(
                    std::max(T(0), T(1) + (T)matrix.m_Numbers[0] + (T)matrix.m_Numbers[5] + (T)matrix.m_Numbers[10])) *
                T(0.5);
            x = std::sqrt(
                    std::max(T(0), T(1) + (T)matrix.m_Numbers[0] - (T)matrix.m_Numbers[5] - (T)matrix.m_Numbers[10])) *
                T(0.5);
            y = std::sqrt(
                    std::max(T(0), T(1) - (T)matrix.m_Numbers[0] + (T)matrix.m_Numbers[5] - (T)matrix.m_Numbers[10])) *
                T(0.5);
            z = std::sqrt(
                    std::max(T(0), T(1) - (T)matrix.m_Numbers[0] - (T)matrix.m_Numbers[5] + (T)matrix.m_Numbers[10])) *
                T(0.5);
            x = std::copysign(x, (T)matrix.m_Numbers[9] - (T)matrix.m_Numbers[6]);
            y = std::copysign(y, (T)matrix.m_Numbers[2] - (T)matrix.m_Numbers[8]);
            z = std::copysign(z, (T)matrix.m_Numbers[4] - (T)matrix.m_Numbers[1]);
        }

        template <typename U>
        Quat<T>(const Quat<U>& other)
        {
            x = (T)other.x;
            y = (T)other.y;
            z = (T)other.z;
            w = (T)other.w;
        }

        inline void Normalize()
        {
            T length = T(1) / Length();
            w *= length;
            x *= length;
            y *= length;
            z *= length;
        }
        inline Quat<T> GetNormalized() const
        {
            T length = T(1) / Length();
            return Quat<T>(w * length, x * length, y * length, z * length);
        }
        inline Quat<T> GetConjugate() const
        {
            return Quat<T>(w, -x, -y, -z);
        }

        T Length() const
        {
            return sqrt(Length2());
        }
        T Length2() const
        {
            return (x * x) + (y * y) + (z * z) + (w * w);
        }
        inline Vec3<T> GetEulerAngles() const
        {
            // roll (z-xis rotation)
            T sinr = T(2.0) * (w * x + y * z);
            T cosr = T(1.0) - T(2.0) * (x * x + y * y);
            T roll = atan2(sinr, cosr);

            // pitch (x-xis rotation)
            T sinp = T(2.0) * (w * y - z * x);
            T pitch = T(0.0);
            if (abs(sinp) >= T(1))
            {
                pitch = copysign(3.1415f * T(0.5), sinp);  // Default to 90 degrees if out of range.
            }
            else
            {
                pitch = asin(sinp);
            }

            // yaw (y-xis rotation)
            T siny = T(2.0) * (w * z + x * y);
            T cosy = T(1.0) - T(2.0) * (y * y + z * z);
            T yaw = atan2(siny, cosy);

            return Vec3<T>(roll, pitch, yaw);
        }

        inline Mat3<T> GetRotationMatrix33() const
        {
            Mat3<T> result;
            T qxx(x * x);
            T qyy(y * y);
            T qzz(z * z);
            T qxz(x * z);
            T qxy(x * y);
            T qyz(y * z);
            T qwx(w * x);
            T qwy(w * y);
            T qwz(w * z);

            result.m_Numbers[0] = T(1) - T(2) * (qyy + qzz);
            result.m_Numbers[3] = T(2) * (qxy + qwz);
            result.m_Numbers[6] = T(2) * (qxz - qwy);

            result.m_Numbers[1] = T(2) * (qxy - qwz);
            result.m_Numbers[4] = T(1) - T(2) * (qxx + qzz);
            result.m_Numbers[7] = T(2) * (qyz + qwx);

            result.m_Numbers[2] = T(2) * (qxz + qwy);
            result.m_Numbers[5] = T(2) * (qyz - qwx);
            result.m_Numbers[8] = T(1) - T(2) * (qxx + qyy);
            return result;
        }

        inline Mat4<T> GetRotationMatrix44() const
        {
            Mat4<T> result;
            T qxx(x * x);
            T qyy(y * y);
            T qzz(z * z);

            T qxz(x * z);
            T qxy(x * y);
            T qyz(y * z);

            T qwx(w * x);
            T qwy(w * y);
            T qwz(w * z);

            result.m_Numbers[0] = T(1) - T(2) * (qyy + qzz);
            result.m_Numbers[4] = T(2) * (qxy + qwz);
            result.m_Numbers[8] = T(2) * (qxz - qwy);

            result.m_Numbers[1] = T(2) * (qxy - qwz);
            result.m_Numbers[5] = T(1) - T(2) * (qxx + qzz);
            result.m_Numbers[9] = T(2) * (qyz + qwx);

            result.m_Numbers[2] = T(2) * (qxz + qwy);
            result.m_Numbers[6] = T(2) * (qyz - qwx);
            result.m_Numbers[10] = T(1) - T(2) * (qxx + qyy);

            return Mat4<T>::Transpose(result);
        }

        inline T Dot(const Quat<T>& quat) const
        {
            return x * quat.x + y * quat.y + z * quat.z + w * quat.w;
        }

        inline Vec3<T> GetForwardVector() const
        {
            return Vec3<T>(2 * (x * z + w * y), 2 * (y * z - w * x), 1 - 2 * (x * x + y * y)).GetNormalized();
        }

        inline Vec3<T> GetUpVector() const
        {
            return Vec3<T>(2 * (x * y - w * z), 1 - 2 * (x * x + z * z), 2 * (y * z + w * x)).GetNormalized();
        }

        inline Vec3<T> GetRightVector() const
        {
            return Vec3<T>(1 - 2 * (y * y + z * z), 2 * (x * y + w * z), 2 * (x * z - w * y)).GetNormalized();
        }

        inline static Quat<T> Slerp(const Quat<T>& a, const Quat<T>& b, const T& delta)
        {
            Quat<T> qz = b;

            T cosTheta = a.Dot(b);

            // If cosTheta < 0, the interpolation will take the long wy around the sphere.
            // To fix this, one quat must be negated.
            if (cosTheta < T(0))
            {
                qz = -qz;
                cosTheta = -cosTheta;
            }

            const T dotThreshold = static_cast<T>(0.9995);
            // Perform a linear interpolation when cosTheta is close to 1 to avoid side effect of sin(angle) becoming a
            // zero denominator
            if (cosTheta > T(1) - dotThreshold)
            {
                // Linear interpolation
                return Lerp(a, qz, delta);
            }
            else
            {
                // Essential Mathematifw, page 467
                T angle = acos(cosTheta);
                return (sin((T(1) - delta) * angle) * a + sin(delta * angle) * qz) / sin(angle);
            }
        }

        template <class T>
        Quat<T> operator*(const T& scalar)
        {
            return Quat<T>(w * scalar, x * scalar, y * scalar, z * scalar);
        }

        template <class T>
        Quat<T> operator*(const Quat<T>& other)
        {
            return Quat<T>((other.w * w) - (other.x * x) - (other.y * y) - (other.z * z),
                           (other.w * x) + (other.x * w) + (other.y * z) - (other.z * y),
                           (other.w * y) + (other.y * w) + (other.z * x) - (other.x * z),
                           (other.w * z) + (other.z * w) + (other.x * y) - (other.y * x));
        }

        template <class T>
        void operator*=(const T& scalar)
        {
            w *= scalar;
            x *= scalar;
            y *= scalar;
            z *= scalar;
        }

        template <class T>
        void operator*=(const Quat<T>& quat1)
        {
            T w = (quat1.w * w) - (quat1.x * x) - (quat1.y * y) - (quat1.z * z);
            T x = (quat1.w * x) + (quat1.x * w) + (quat1.y * z) - (quat1.z * y);
            T y = (quat1.w * y) + (quat1.y * w) + (quat1.z * x) - (quat1.x * z);
            T z = (quat1.w * z) + (quat1.z * w) + (quat1.x * y) - (quat1.y * x);
        }

        template <class T>
        Quat<T> operator/(const T& scalar)
        {
            return Quat<T>(w / scalar, x / scalar, y / scalar, z / scalar);
        }

        template <class T>
        Quat<T> operator-(const Quat<T>& b)
        {
            return Quat<T>(w - b.w, x - b.x, y - b.y, z - b.z);
        }

        template <class T>
        Quat<T> operator+(const Quat<T>& b)
        {
            return Quat<T>(w + b.w, x + b.x, y + b.y, z + b.z);
        }

        template <class T>
        void operator+=(const Quat<T>& b)
        {
            w += b.w;
            x += b.x;
            y += b.y;
            z += b.z;
        }

        template <class T>
        bool operator==(const Quat<T>& b)
        {
            return x == b.x && y == b.y && z == b.z && w == b.w;
        }

        template <class T>
        bool operator!=(const Quat<T>& b)
        {
            return !(*this == b);
        }

        union
        {
            T values[4];
            struct
            {
                T w;
                T x;
                T y;
                T z;
            };
        };
    };

    typedef Quat<f32> Quatf;
}  // namespace fw