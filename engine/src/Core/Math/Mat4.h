#pragma once

#include <cassert>
#include <cmath>
#include <initializer_list>

#include <dubu_serialize/dubu_serialize.h>
#include <xmmintrin.h>

#include "Core/Types.h"
#include "Quat.h"
#include "Vec3.h"
#include "Vec4.h"

namespace fw
{
    template <typename T>
    class Mat3;

    template <typename T>
    class Mat4
    {
    public:
        friend class Mat3<T>;

        Mat4()
            : m_Numbers{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 }
        { }

        Mat4(const std::initializer_list<T>& initList)
        {
            assert(initList.size() == 16 && "Initializer list for Matrix44 must contain exactly 16 elements.");
            auto begin = initList.begin();
            for (size_t i = 0; i < 16; ++i)
            {
                m_Numbers[i] = *(begin + i);
            }
        }

        inline void Visualize() const
        {
            INFO_LOG(
                "\n[{:.2f}, {:.2f}, {:.2f}, {:.2f}]"
                "\n[{:.2f}, {:.2f}, {:.2f}, {:.2f}]"
                "\n[{:.2f}, {:.2f}, {:.2f}, {:.2f}]"
                "\n[{:.2f}, {:.2f}, {:.2f}, {:.2f}]",
                m_Numbers[0],
                m_Numbers[1],
                m_Numbers[2],
                m_Numbers[3],
                m_Numbers[4],
                m_Numbers[5],
                m_Numbers[6],
                m_Numbers[7],
                m_Numbers[8],
                m_Numbers[9],
                m_Numbers[10],
                m_Numbers[11],
                m_Numbers[12],
                m_Numbers[13],
                m_Numbers[14],
                m_Numbers[15]);
        }

        Mat4(const Mat4<T>& other)
        {
            *this = other;
        }

        inline Mat4<T>& operator=(const Mat4<T>& other)
        {
            for (i32 i = 0; i < 16; i++)
            {
                m_Numbers[i] = other.m_Numbers[i];
            }
            return *this;
        }

        Mat4(Mat4<T>&& other)
        {
            *this = other;
        }

        inline Mat4<T>& operator=(Mat4<T>&& other) noexcept
        {
            for (i32 i = 0; i < 16; i++)
            {
                m_Numbers[i] = other.m_Numbers[i];
            }
            return *this;
        }

        inline Mat4<T> operator+(const Mat4<T>& other) const
        {
            Mat4<T> result;
            for (size_t i = 0; i < 16; ++i)
            {
                result.m_Numbers[i] = m_Numbers[i] + other.m_Numbers[i];
            }
            return result;
        }

        inline Mat4<T> operator-(const Mat4<T>& other) const
        {
            Mat4<T> result;
            for (size_t i = 0; i < 16; ++i)
            {
                result.m_Numbers[i] = m_Numbers[i] - other.m_Numbers[i];
            }
            return result;
        }

        inline Mat4<T> operator*(const Mat4<T>& other) const
        {
            Mat4<T> result;
            for (size_t i = 0; i < 4; i++)
            {
                __m128 ax = _mm_set1_ps(m_Numbers[i * 4]);
                __m128 ay = _mm_set1_ps(m_Numbers[i * 4 + 1]);
                __m128 az = _mm_set1_ps(m_Numbers[i * 4 + 2]);
                __m128 aw = _mm_set1_ps(m_Numbers[i * 4 + 3]);

                __m128 x = _mm_mul_ps(ax, other.m_Rows[0]);
                __m128 y = _mm_mul_ps(ay, other.m_Rows[1]);
                __m128 z = _mm_mul_ps(az, other.m_Rows[2]);
                __m128 w = _mm_mul_ps(aw, other.m_Rows[3]);

                result.m_Rows[i] = _mm_add_ps(x, _mm_add_ps(y, _mm_add_ps(z, w)));
            }
            return result;
        }

        inline Mat4<T> operator*(const T& scalar) const
        {
            Mat4<T> result;
            for (size_t i = 0; i < 16; ++i)
            {
                result.m_Numbers[i] = m_Numbers[i] * scalar;
            }
            return result;
        }

        inline bool operator==(const Mat4<T>& other) const
        {
            for (size_t i = 0; i < 16; ++i)
            {
                if (m_Numbers[i] != other.m_Numbers[i])
                    return false;
            }
            return true;
        }

        inline void operator+=(const Mat4<T>& other)
        {
            *this = *this + other;
        }

        inline void operator-=(const Mat4<T>& other)
        {
            *this = *this - other;
        }

        inline void operator*=(const Mat4<T>& other)
        {
            *this = *this * other;
        }

        inline void operator*=(const T& scalar)
        {
            *this = *this * scalar;
        }

        inline bool operator!=(const Mat4<T>& other) const
        {
            return !(*this == other);
        }

        inline T& operator[](const u32& index)
        {
            assert(index < 16 && "Index out of bounds.");
            return m_Numbers[index];
        }

        inline const T& operator[](const u32& index) const
        {
            assert(index < 16 && "Index out of bounds.");
            return m_Numbers[index];
        }

        static Mat4<T> CreateOrthographicProjection(T left, T right, T bottom, T top, T znear, T zfar)
        {
            f32 ReciprocalWidth = 1.0f / (right - left);
            f32 ReciprocalHeight = 1.0f / (top - bottom);
            f32 fRange = 1.0f / (zfar - znear);
            return Mat4<T>{ ReciprocalWidth + ReciprocalWidth,
                            0,
                            0,
                            0,
                            0,
                            ReciprocalHeight + ReciprocalHeight,
                            0,
                            0,
                            0,
                            0,
                            fRange,
                            0,
                            -(left + right) * ReciprocalWidth,
                            -(top + bottom) * ReciprocalHeight,
                            -fRange * znear,
                            1 };
        }

        static Mat4<T> CreateOrthographicProjection(T width, T height, T znear, T zfar)
        {
            T B = (T)2.0 / height;
            T A = (T)2.0 / width;
            T C = (T)1.0 / (zfar - znear);
            T E = znear / (znear - zfar);
            return Mat4<T>{ A, 0, 0, 0, 0, B, 0, 0, 0, 0, C, 0, 0, 0, E, 1 };
        }

        static Mat4<T> CreatePerspectiveProjection(T fov, T aspect, T znear, T zfar)
        {
            T yFov = fov / aspect;
            T B = (T)1.0f / (std::tan(yFov * (3.1415f / 180.0f) * (T)0.5f));
            T A = B / aspect;
            T C = zfar / (zfar - znear);
            T D = (T)1.0f;
            T E = -znear * zfar / (zfar - znear);
            return Mat4<T>{ A, 0, 0, 0, 0, B, 0, 0, 0, 0, C, D, 0, 0, E, 0 };
        }

        inline static Mat4<T> CreateRotationAroundX(T angle)
        {
            T c = cos(angle);
            T s = sin(angle);
            return Mat4<T>({ 1, 0, 0, 0, 0, c, s, 0, 0, -s, c, 0, 0, 0, 0, 1 });
        }

        inline static Mat4<T> CreateRotationAroundY(T angle)
        {
            T c = cos(angle);
            T s = sin(angle);
            return Mat4<T>({ c, 0, -s, 0, 0, 1, 0, 0, s, 0, c, 0, 0, 0, 0, 1 });
        }

        inline static Mat4<T> CreateRotationAroundZ(T angle)
        {
            T c = cos(angle);
            T s = sin(angle);
            return Mat4<T>({ c, s, 0, 0, -s, c, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 });
        }

        inline static Mat4<T> FastInverse(const Mat4<T>& matrix)
        {
            Mat3<T> transposed(matrix);
            transposed = Mat3<T>::Transpose(transposed);
            Vec3<T> negated(-matrix.m_Numbers[12], -matrix.m_Numbers[13], -matrix.m_Numbers[14]);
            negated *= transposed;
            return Mat4<T>({ transposed.m_Numbers[0],
                             transposed.m_Numbers[1],
                             transposed.m_Numbers[2],
                             0,
                             transposed.m_Numbers[3],
                             transposed.m_Numbers[4],
                             transposed.m_Numbers[5],
                             0,
                             transposed.m_Numbers[6],
                             transposed.m_Numbers[7],
                             transposed.m_Numbers[8],
                             0,
                             negated.x,
                             negated.y,
                             negated.z,
                             1 });
        }

        inline static Mat4<T> Rotate(const Mat4<T>& matrix, f32 angle, Vec3<T> rotation_axis)
        {
            Mat4<T> r = CreateRotationAroundZ(angle * rotation_axis.z) *
                        CreateRotationAroundY(angle * rotation_axis.y) * CreateRotationAroundX(angle * rotation_axis.x);

            return matrix * r;
        }

        inline static Mat4<T> Translate(const Mat4<T>& matrix, Vec3<T> translation)
        {
            Mat4<T> t = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, translation.x, translation.y, translation.z, 1 };

            return matrix * t;
        }

        inline static Mat4<T> Scale(const Mat4<T>& matrix, Vec3<T> scale)
        {
            Mat4<T> s = {
                scale.x, 0, 0, 0, 0, scale.y, 0, 0, 0, 0, scale.z, 0, 0, 0, 0, 1,
            };

            return matrix * s;
        }

        inline static Mat4<T> CreateTransform(const Vec3<T>& position, const Vec3<T>& rotation, const Vec3<T>& scale)
        {
            Mat4<T> s = {
                scale.x, 0, 0, 0, 0, scale.y, 0, 0, 0, 0, scale.z, 0, 0, 0, 0, 1,
            };

            Mat4<T> r = CreateRotationAroundZ(rotation.z) * CreateRotationAroundY(rotation.y) *
                        CreateRotationAroundX(rotation.x);

            Mat4<T> t = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, position.x, position.y, position.z, 1 };

            return s * r * t;
        }

        inline static Mat4<T> CreateTranslationMatrix(const T x, const T y, const T z)
        {
            Mat4<T> result;
            result.m_Position.x = x;
            result.m_Position.y = y;
            result.m_Position.z = z;
            return result;
        }

        inline static Mat4<T> CreateLookAt(const Vec3<T>& lookAt, const Vec3<T>& eye, const Vec3<T>& up)
        {
            Vec3<T> zaxis = (lookAt - eye).GetNormalized();
            Vec3<T> xaxis = (up.Cross(zaxis)).GetNormalized();
            Vec3<T> yaxis = zaxis.Cross(xaxis);
            return Mat4<T>{ xaxis.x, yaxis.x, zaxis.x, 0, xaxis.y,         yaxis.y,         zaxis.y,         0,
                            xaxis.z, yaxis.z, zaxis.z, 0, -xaxis.Dot(eye), -yaxis.Dot(eye), -zaxis.Dot(eye), 1 };
        }

        inline static Mat4<T> Transpose(const Mat4<T>& matrix)
        {
            Mat4<T> result(matrix);

            result[1] = matrix[4];
            result[4] = matrix[1];

            result[2] = matrix[8];
            result[8] = matrix[2];

            result[3] = matrix[12];
            result[12] = matrix[3];

            result[6] = matrix[9];
            result[9] = matrix[6];

            result[7] = matrix[13];
            result[13] = matrix[7];

            result[11] = matrix[14];
            result[14] = matrix[11];

            return result;
        }

        inline static T Determinant(const Mat4<T>& matrix)
        {
            return matrix.m_Numbers[0] * ((matrix.m_Numbers[5] * matrix.m_Numbers[10] * matrix.m_Numbers[15] +
                                           matrix.m_Numbers[6] * matrix.m_Numbers[11] * matrix.m_Numbers[13] +
                                           matrix.m_Numbers[9] * matrix.m_Numbers[14] * matrix.m_Numbers[7]) -
                                          (matrix.m_Numbers[7] * matrix.m_Numbers[10] * matrix.m_Numbers[13] +
                                           matrix.m_Numbers[11] * matrix.m_Numbers[14] * matrix.m_Numbers[5] +
                                           matrix.m_Numbers[6] * matrix.m_Numbers[9] * matrix.m_Numbers[15])) -
                   matrix.m_Numbers[1] * ((matrix.m_Numbers[4] * matrix.m_Numbers[10] * matrix.m_Numbers[15] +
                                           matrix.m_Numbers[6] * matrix.m_Numbers[11] * matrix.m_Numbers[12] +
                                           matrix.m_Numbers[8] * matrix.m_Numbers[14] * matrix.m_Numbers[7]) -
                                          (matrix.m_Numbers[7] * matrix.m_Numbers[10] * matrix.m_Numbers[12] +
                                           matrix.m_Numbers[11] * matrix.m_Numbers[14] * matrix.m_Numbers[4] +
                                           matrix.m_Numbers[6] * matrix.m_Numbers[8] * matrix.m_Numbers[15])) +
                   matrix.m_Numbers[2] * ((matrix.m_Numbers[4] * matrix.m_Numbers[9] * matrix.m_Numbers[15] +
                                           matrix.m_Numbers[5] * matrix.m_Numbers[11] * matrix.m_Numbers[12] +
                                           matrix.m_Numbers[8] * matrix.m_Numbers[13] * matrix.m_Numbers[7]) -
                                          (matrix.m_Numbers[7] * matrix.m_Numbers[9] * matrix.m_Numbers[12] +
                                           matrix.m_Numbers[11] * matrix.m_Numbers[13] * matrix.m_Numbers[4] +
                                           matrix.m_Numbers[5] * matrix.m_Numbers[8] * matrix.m_Numbers[15])) -
                   matrix.m_Numbers[3] * ((matrix.m_Numbers[4] * matrix.m_Numbers[9] * matrix.m_Numbers[14] +
                                           matrix.m_Numbers[5] * matrix.m_Numbers[10] * matrix.m_Numbers[12] +
                                           matrix.m_Numbers[8] * matrix.m_Numbers[13] * matrix.m_Numbers[6]) -
                                          (matrix.m_Numbers[6] * matrix.m_Numbers[9] * matrix.m_Numbers[12] +
                                           matrix.m_Numbers[10] * matrix.m_Numbers[13] * matrix.m_Numbers[4] +
                                           matrix.m_Numbers[5] * matrix.m_Numbers[8] * matrix.m_Numbers[14]));
        }

        inline static Mat4<T> Inverse(const Mat4<T>& matrix)
        {
            T det = Determinant(matrix);
            assert(det != 0 && "Non-invertible matrix");

            Mat4<T> result;
            result.m_Numbers[0] = matrix.m_Numbers[5] * matrix.m_Numbers[10] * matrix.m_Numbers[15] -
                                  matrix.m_Numbers[5] * matrix.m_Numbers[11] * matrix.m_Numbers[14] -
                                  matrix.m_Numbers[9] * matrix.m_Numbers[6] * matrix.m_Numbers[15] +
                                  matrix.m_Numbers[9] * matrix.m_Numbers[7] * matrix.m_Numbers[14] +
                                  matrix.m_Numbers[13] * matrix.m_Numbers[6] * matrix.m_Numbers[11] -
                                  matrix.m_Numbers[13] * matrix.m_Numbers[7] * matrix.m_Numbers[10];

            result.m_Numbers[4] = -matrix.m_Numbers[4] * matrix.m_Numbers[10] * matrix.m_Numbers[15] +
                                  matrix.m_Numbers[4] * matrix.m_Numbers[11] * matrix.m_Numbers[14] +
                                  matrix.m_Numbers[8] * matrix.m_Numbers[6] * matrix.m_Numbers[15] -
                                  matrix.m_Numbers[8] * matrix.m_Numbers[7] * matrix.m_Numbers[14] -
                                  matrix.m_Numbers[12] * matrix.m_Numbers[6] * matrix.m_Numbers[11] +
                                  matrix.m_Numbers[12] * matrix.m_Numbers[7] * matrix.m_Numbers[10];

            result.m_Numbers[8] = matrix.m_Numbers[4] * matrix.m_Numbers[9] * matrix.m_Numbers[15] -
                                  matrix.m_Numbers[4] * matrix.m_Numbers[11] * matrix.m_Numbers[13] -
                                  matrix.m_Numbers[8] * matrix.m_Numbers[5] * matrix.m_Numbers[15] +
                                  matrix.m_Numbers[8] * matrix.m_Numbers[7] * matrix.m_Numbers[13] +
                                  matrix.m_Numbers[12] * matrix.m_Numbers[5] * matrix.m_Numbers[11] -
                                  matrix.m_Numbers[12] * matrix.m_Numbers[7] * matrix.m_Numbers[9];

            result.m_Numbers[12] = -matrix.m_Numbers[4] * matrix.m_Numbers[9] * matrix.m_Numbers[14] +
                                   matrix.m_Numbers[4] * matrix.m_Numbers[10] * matrix.m_Numbers[13] +
                                   matrix.m_Numbers[8] * matrix.m_Numbers[5] * matrix.m_Numbers[14] -
                                   matrix.m_Numbers[8] * matrix.m_Numbers[6] * matrix.m_Numbers[13] -
                                   matrix.m_Numbers[12] * matrix.m_Numbers[5] * matrix.m_Numbers[10] +
                                   matrix.m_Numbers[12] * matrix.m_Numbers[6] * matrix.m_Numbers[9];

            result.m_Numbers[1] = -matrix.m_Numbers[1] * matrix.m_Numbers[10] * matrix.m_Numbers[15] +
                                  matrix.m_Numbers[1] * matrix.m_Numbers[11] * matrix.m_Numbers[14] +
                                  matrix.m_Numbers[9] * matrix.m_Numbers[2] * matrix.m_Numbers[15] -
                                  matrix.m_Numbers[9] * matrix.m_Numbers[3] * matrix.m_Numbers[14] -
                                  matrix.m_Numbers[13] * matrix.m_Numbers[2] * matrix.m_Numbers[11] +
                                  matrix.m_Numbers[13] * matrix.m_Numbers[3] * matrix.m_Numbers[10];

            result.m_Numbers[5] = matrix.m_Numbers[0] * matrix.m_Numbers[10] * matrix.m_Numbers[15] -
                                  matrix.m_Numbers[0] * matrix.m_Numbers[11] * matrix.m_Numbers[14] -
                                  matrix.m_Numbers[8] * matrix.m_Numbers[2] * matrix.m_Numbers[15] +
                                  matrix.m_Numbers[8] * matrix.m_Numbers[3] * matrix.m_Numbers[14] +
                                  matrix.m_Numbers[12] * matrix.m_Numbers[2] * matrix.m_Numbers[11] -
                                  matrix.m_Numbers[12] * matrix.m_Numbers[3] * matrix.m_Numbers[10];

            result.m_Numbers[9] = -matrix.m_Numbers[0] * matrix.m_Numbers[9] * matrix.m_Numbers[15] +
                                  matrix.m_Numbers[0] * matrix.m_Numbers[11] * matrix.m_Numbers[13] +
                                  matrix.m_Numbers[8] * matrix.m_Numbers[1] * matrix.m_Numbers[15] -
                                  matrix.m_Numbers[8] * matrix.m_Numbers[3] * matrix.m_Numbers[13] -
                                  matrix.m_Numbers[12] * matrix.m_Numbers[1] * matrix.m_Numbers[11] +
                                  matrix.m_Numbers[12] * matrix.m_Numbers[3] * matrix.m_Numbers[9];

            result.m_Numbers[13] = matrix.m_Numbers[0] * matrix.m_Numbers[9] * matrix.m_Numbers[14] -
                                   matrix.m_Numbers[0] * matrix.m_Numbers[10] * matrix.m_Numbers[13] -
                                   matrix.m_Numbers[8] * matrix.m_Numbers[1] * matrix.m_Numbers[14] +
                                   matrix.m_Numbers[8] * matrix.m_Numbers[2] * matrix.m_Numbers[13] +
                                   matrix.m_Numbers[12] * matrix.m_Numbers[1] * matrix.m_Numbers[10] -
                                   matrix.m_Numbers[12] * matrix.m_Numbers[2] * matrix.m_Numbers[9];

            result.m_Numbers[2] = matrix.m_Numbers[1] * matrix.m_Numbers[6] * matrix.m_Numbers[15] -
                                  matrix.m_Numbers[1] * matrix.m_Numbers[7] * matrix.m_Numbers[14] -
                                  matrix.m_Numbers[5] * matrix.m_Numbers[2] * matrix.m_Numbers[15] +
                                  matrix.m_Numbers[5] * matrix.m_Numbers[3] * matrix.m_Numbers[14] +
                                  matrix.m_Numbers[13] * matrix.m_Numbers[2] * matrix.m_Numbers[7] -
                                  matrix.m_Numbers[13] * matrix.m_Numbers[3] * matrix.m_Numbers[6];

            result.m_Numbers[6] = -matrix.m_Numbers[0] * matrix.m_Numbers[6] * matrix.m_Numbers[15] +
                                  matrix.m_Numbers[0] * matrix.m_Numbers[7] * matrix.m_Numbers[14] +
                                  matrix.m_Numbers[4] * matrix.m_Numbers[2] * matrix.m_Numbers[15] -
                                  matrix.m_Numbers[4] * matrix.m_Numbers[3] * matrix.m_Numbers[14] -
                                  matrix.m_Numbers[12] * matrix.m_Numbers[2] * matrix.m_Numbers[7] +
                                  matrix.m_Numbers[12] * matrix.m_Numbers[3] * matrix.m_Numbers[6];

            result.m_Numbers[10] = matrix.m_Numbers[0] * matrix.m_Numbers[5] * matrix.m_Numbers[15] -
                                   matrix.m_Numbers[0] * matrix.m_Numbers[7] * matrix.m_Numbers[13] -
                                   matrix.m_Numbers[4] * matrix.m_Numbers[1] * matrix.m_Numbers[15] +
                                   matrix.m_Numbers[4] * matrix.m_Numbers[3] * matrix.m_Numbers[13] +
                                   matrix.m_Numbers[12] * matrix.m_Numbers[1] * matrix.m_Numbers[7] -
                                   matrix.m_Numbers[12] * matrix.m_Numbers[3] * matrix.m_Numbers[5];

            result.m_Numbers[14] = -matrix.m_Numbers[0] * matrix.m_Numbers[5] * matrix.m_Numbers[14] +
                                   matrix.m_Numbers[0] * matrix.m_Numbers[6] * matrix.m_Numbers[13] +
                                   matrix.m_Numbers[4] * matrix.m_Numbers[1] * matrix.m_Numbers[14] -
                                   matrix.m_Numbers[4] * matrix.m_Numbers[2] * matrix.m_Numbers[13] -
                                   matrix.m_Numbers[12] * matrix.m_Numbers[1] * matrix.m_Numbers[6] +
                                   matrix.m_Numbers[12] * matrix.m_Numbers[2] * matrix.m_Numbers[5];

            result.m_Numbers[3] = -matrix.m_Numbers[1] * matrix.m_Numbers[6] * matrix.m_Numbers[11] +
                                  matrix.m_Numbers[1] * matrix.m_Numbers[7] * matrix.m_Numbers[10] +
                                  matrix.m_Numbers[5] * matrix.m_Numbers[2] * matrix.m_Numbers[11] -
                                  matrix.m_Numbers[5] * matrix.m_Numbers[3] * matrix.m_Numbers[10] -
                                  matrix.m_Numbers[9] * matrix.m_Numbers[2] * matrix.m_Numbers[7] +
                                  matrix.m_Numbers[9] * matrix.m_Numbers[3] * matrix.m_Numbers[6];

            result.m_Numbers[7] = matrix.m_Numbers[0] * matrix.m_Numbers[6] * matrix.m_Numbers[11] -
                                  matrix.m_Numbers[0] * matrix.m_Numbers[7] * matrix.m_Numbers[10] -
                                  matrix.m_Numbers[4] * matrix.m_Numbers[2] * matrix.m_Numbers[11] +
                                  matrix.m_Numbers[4] * matrix.m_Numbers[3] * matrix.m_Numbers[10] +
                                  matrix.m_Numbers[8] * matrix.m_Numbers[2] * matrix.m_Numbers[7] -
                                  matrix.m_Numbers[8] * matrix.m_Numbers[3] * matrix.m_Numbers[6];

            result.m_Numbers[11] = -matrix.m_Numbers[0] * matrix.m_Numbers[5] * matrix.m_Numbers[11] +
                                   matrix.m_Numbers[0] * matrix.m_Numbers[7] * matrix.m_Numbers[9] +
                                   matrix.m_Numbers[4] * matrix.m_Numbers[1] * matrix.m_Numbers[11] -
                                   matrix.m_Numbers[4] * matrix.m_Numbers[3] * matrix.m_Numbers[9] -
                                   matrix.m_Numbers[8] * matrix.m_Numbers[1] * matrix.m_Numbers[7] +
                                   matrix.m_Numbers[8] * matrix.m_Numbers[3] * matrix.m_Numbers[5];

            result.m_Numbers[15] = matrix.m_Numbers[0] * matrix.m_Numbers[5] * matrix.m_Numbers[10] -
                                   matrix.m_Numbers[0] * matrix.m_Numbers[6] * matrix.m_Numbers[9] -
                                   matrix.m_Numbers[4] * matrix.m_Numbers[1] * matrix.m_Numbers[10] +
                                   matrix.m_Numbers[4] * matrix.m_Numbers[2] * matrix.m_Numbers[9] +
                                   matrix.m_Numbers[8] * matrix.m_Numbers[1] * matrix.m_Numbers[6] -
                                   matrix.m_Numbers[8] * matrix.m_Numbers[2] * matrix.m_Numbers[5];
            return result * (1 / det);
        }

        inline Mat4<T> GetInversed() const
        {
            return Inverse(*this);
        }

        Vec3<T> GetRotation() const
        {
            Vec3<T> output;
            output.z = atan2(_21, _11);
            output.y = -asin(_31);
            output.x = atan2(_32, _33);
            return output;
        }

        friend inline Vec4<T> operator*(const Vec4<T>& vector, const Mat4<T>& matrix)
        {
            return Vec4<T>(vector.x * matrix.m_Numbers[0] + vector.y * matrix.m_Numbers[4] +
                               vector.z * matrix.m_Numbers[8] + vector.w * matrix.m_Numbers[12],
                           vector.x * matrix.m_Numbers[1] + vector.y * matrix.m_Numbers[5] +
                               vector.z * matrix.m_Numbers[9] + vector.w * matrix.m_Numbers[13],
                           vector.x * matrix.m_Numbers[2] + vector.y * matrix.m_Numbers[6] +
                               vector.z * matrix.m_Numbers[10] + vector.w * matrix.m_Numbers[14],
                           vector.x * matrix.m_Numbers[3] + vector.y * matrix.m_Numbers[7] +
                               vector.z * matrix.m_Numbers[11] + vector.w * matrix.m_Numbers[15]);
        }
        friend inline void operator*=(Vec4<T>& vector, const Mat4<T>& matrix)
        {
            vector = vector * matrix;
        };

        friend inline Vec3<T> operator*(const Vec3<T>& vector, const Mat4<T>& matrix)
        {
            return Vec3<T>(vector.x * matrix.m_Numbers[0] + vector.y * matrix.m_Numbers[4] +
                               vector.z * matrix.m_Numbers[8] + matrix.m_Numbers[12],
                           vector.x * matrix.m_Numbers[1] + vector.y * matrix.m_Numbers[5] +
                               vector.z * matrix.m_Numbers[9] + matrix.m_Numbers[13],
                           vector.x * matrix.m_Numbers[2] + vector.y * matrix.m_Numbers[6] +
                               vector.z * matrix.m_Numbers[10] + matrix.m_Numbers[14]);
        }
        friend inline void operator*=(Vec3<T>& vector, const Mat4<T>& matrix)
        {
            vector = vector * matrix;
        };

        void Serialize(dubu::serialize::ReadBuffer& buffer)
        {
            buffer >> m_RightAxis >> m_RightW >> m_UpAxis >> m_UpW >> m_ForwardAxis >> m_ForwardW >> m_Position >> m_W;
        }
        void Serialize(dubu::serialize::WriteBuffer& buffer) const
        {
            buffer << m_RightAxis << m_RightW << m_UpAxis << m_UpW << m_ForwardAxis << m_ForwardW << m_Position << m_W;
        }

        union
        {
            struct
            {
                Vec3<T> m_RightAxis;
                T m_RightW;
                Vec3<T> m_UpAxis;
                T m_UpW;
                Vec3<T> m_ForwardAxis;
                T m_ForwardW;
                Vec3<T> m_Position;
                T m_W;
            };
            struct
            {
                T _11, _12, _13, _14;
                T _21, _22, _23, _24;
                T _31, _32, _33, _34;
                T _41, _42, _43, _44;
            };
            struct
            {
                T m_Numbers[16];
            };
            struct
            {
                __m128 m_Rows[4];
            };
        };
    };

    using Mat4f = Mat4<f32>;
}  // namespace fw