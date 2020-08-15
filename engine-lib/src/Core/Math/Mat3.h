#pragma once
#include "../Types.h"
#include "Vec2.h"
#include "Vec3.h"
#include "Mat4.h"
#include <initializer_list>

namespace fw
{
	template<typename T>
	class Mat3
	{
	public:

		friend class Mat4<T>;
		Mat3() : m_Numbers{ 1, 0, 0, 0, 1, 0, 0, 0, 1 }
		{
		}
		Mat3(const std::initializer_list<T>& initList)
		{
			assert(initList.size() == 9 && "Initializer list for Mat3 must contain exactly 9 elements.");
			auto begin = initList.begin();
			for (size_t i = 0; i < 9; ++i)
			{
				m_Numbers[i] = *(begin + i);
			}
		}
		Mat3(const Mat3<T>& matrix)
		{
			*this = matrix;
		}
		Mat3(const Mat4<T>& matrix)
		{
			m_Numbers[0] = matrix.m_Numbers[0];
			m_Numbers[1] = matrix.m_Numbers[1];
			m_Numbers[2] = matrix.m_Numbers[2];

			m_Numbers[3] = matrix.m_Numbers[4];
			m_Numbers[4] = matrix.m_Numbers[5];
			m_Numbers[5] = matrix.m_Numbers[6];

			m_Numbers[6] = matrix.m_Numbers[8];
			m_Numbers[7] = matrix.m_Numbers[9];
			m_Numbers[8] = matrix.m_Numbers[10];

		}

		static Mat3<T> CreateRotationAroundX(T angle)
		{
			T c = cos(angle);
			T s = sin(angle);
			return Mat3<T>({
				1, 0, 0,
				0, c, s,
				0, -s, c
				});
		}

		static Mat3<T> CreateRotationAroundY(T angle)
		{
			T c = cos(angle);
			T s = sin(angle);
			return Mat3<T>({
				c, 0, -s,
				0, 1, 0,
				s, 0, c
				});
		}

		static Mat3<T> CreateRotationAroundZ(T angle)
		{
			T c = cos(angle);
			T s = sin(angle);
			return Mat3<T>({
				c, s, 0,
				-s, c, 0,
				0, 0, 1
				});
		}

		static Mat3<T> CreateTransform(const Vec2<T>& position, const f32 rotation, const Vec2<T>& scale)
		{
			Mat3<T> s = {
				scale.x, 0, 0,
				0, scale.y, 0,
				0, 0, 1,
			};

			fw::Mat3<T> r = fw::Mat3<T>::CreateRotationAroundZ(rotation);

			fw::Mat3<T> t = {
				1, 0, 0,
				0, 1, 0,
				position.x, position.y, 1
			};
			return s * r * t;
		}

		static Mat3<T> Transpose(const Mat3<T>& matrix)
		{
			Mat3<T> result(matrix);

			result.m_Numbers[1] = matrix.m_Numbers[3];
			result.m_Numbers[3] = matrix.m_Numbers[1];

			result.m_Numbers[2] = matrix.m_Numbers[6];
			result.m_Numbers[6] = matrix.m_Numbers[2];

			result.m_Numbers[5] = matrix.m_Numbers[7];
			result.m_Numbers[7] = matrix.m_Numbers[5];

			return result;

		}
		static T Determinant(const Mat3<T>& matrix)
		{
			return (matrix.m_Numbers[0] * matrix.m_Numbers[4] * matrix.m_Numbers[8] +
				matrix.m_Numbers[1] * matrix.m_Numbers[5] * matrix.m_Numbers[6] +
				matrix.m_Numbers[3] * matrix.m_Numbers[7] * matrix.m_Numbers[2]) -
				(matrix.m_Numbers[2] * matrix.m_Numbers[4] * matrix.m_Numbers[6] +
					matrix.m_Numbers[5] * matrix.m_Numbers[7] * matrix.m_Numbers[0] +
					matrix.m_Numbers[1] * matrix.m_Numbers[3] * matrix.m_Numbers[8]);
		}
		static Mat3<T> Inverse(const Mat3<T>& matrix)
		{
			T det = Determinant(matrix);
			assert(det != 0 && "Non-invertible matrix");
			Mat3<T> result{
				matrix.m_Numbers[4] * matrix.m_Numbers[8] - matrix.m_Numbers[5] * matrix.m_Numbers[7],
				-(matrix.m_Numbers[1] * matrix.m_Numbers[8] - matrix.m_Numbers[2] * matrix.m_Numbers[7]),
				matrix.m_Numbers[1] * matrix.m_Numbers[5] - matrix.m_Numbers[2] * matrix.m_Numbers[4],
				-(matrix.m_Numbers[3] * matrix.m_Numbers[8] - matrix.m_Numbers[5] * matrix.m_Numbers[6]),
				matrix.m_Numbers[0] * matrix.m_Numbers[8] - matrix.m_Numbers[2] * matrix.m_Numbers[6],
				-(matrix.m_Numbers[0] * matrix.m_Numbers[5] - matrix.m_Numbers[2] * matrix.m_Numbers[3]),
				matrix.m_Numbers[3] * matrix.m_Numbers[7] - matrix.m_Numbers[4] * matrix.m_Numbers[6],
				-(matrix.m_Numbers[0] * matrix.m_Numbers[7] - matrix.m_Numbers[1] * matrix.m_Numbers[6]),
				matrix.m_Numbers[0] * matrix.m_Numbers[4] - matrix.m_Numbers[1] * matrix.m_Numbers[3] };
			return result * (1 / det);
		}

		inline Mat3<T>& operator=(const Mat3<T>& matrix)
		{
			for (size_t i = 0; i < 9; ++i)
			{
				m_Numbers[i] = matrix.m_Numbers[i];
			}
			return *this;

		}
		inline Mat3<T> operator+(const Mat3<T>& matrix) const
		{
			Mat3<T> result;
			for (size_t i = 0; i < 9; i++)
			{
				result.m_Numbers[i] = m_Numbers[i] + matrix.m_Numbers[i];
			}
			return result;
		}
		inline Mat3<T> operator-(const Mat3<T>& matrix) const
		{
			Mat3<T> result;
			for (size_t i = 0; i < 9; i++)
			{
				result.m_Numbers[i] = m_Numbers[i] - matrix.m_Numbers[i];
			}
			return result;
		}
		inline Mat3<T> operator*(const Mat3<T>& matrix) const
		{
			Mat3<T> result;
			for (size_t i = 0; i <= 6; i += 3)
			{
				for (size_t j = 0; j < 3; j++)
				{
					result.m_Numbers[i + j] = m_Numbers[i] * matrix.m_Numbers[j] + m_Numbers[1 + i] * matrix.m_Numbers[3 + j] + m_Numbers[2 + i] * matrix.m_Numbers[6 + j];
				}
			}
			return result;
		}
		inline Mat3<T> operator*(const T& aScalar) const
		{
			Mat3<T> result;
			for (size_t i = 0; i < 9; i++)
			{
				result.m_Numbers[i] = m_Numbers[i] * aScalar;
			}
			return result;
		}
		inline bool operator==(const Mat3<T>& matrix) const
		{
			for (size_t i = 0; i < 9; i++)
			{
				if (m_Numbers[i] != matrix.m_Numbers[i])
				{
					return false;
				}
			}
			return true;
		}

		inline void operator+=(const Mat3<T>& matrix) { *this = *this + matrix; }
		inline void operator-=(const Mat3<T>& matrix) { *this = *this - matrix; }
		inline void operator*=(const Mat3<T>& matrix) { *this = *this * matrix; }
		inline void operator*=(const T& aScalar) { *this = *this * aScalar; };

		friend inline Vec3<T> operator*(const Vec3<T>& vec, const Mat3<T>& matrix)
		{
			return Vec3<T>(
				vec.x * matrix.m_Numbers[0] + vec.y * matrix.m_Numbers[3] + vec.z * matrix.m_Numbers[6],
				vec.x * matrix.m_Numbers[1] + vec.y * matrix.m_Numbers[4] + vec.z * matrix.m_Numbers[7],
				vec.x * matrix.m_Numbers[2] + vec.y * matrix.m_Numbers[5] + vec.z * matrix.m_Numbers[8]
				);
		}
		friend inline void operator*=(Vec3<T>& aVector, const Mat3<T>& matrix) { aVector = aVector * matrix; };

		T m_Numbers[9];
	};
	using Mat3f = Mat3<f32>;
	using Mat3i = Mat3<i32>;
}