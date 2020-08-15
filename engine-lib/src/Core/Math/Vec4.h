#pragma once
#include "Vec3.h"
#pragma warning(disable: 4201)

namespace fw
{
	template<typename T>
	class Vec4
	{
	public:
		Vec4(T val) : x(val), y(val), z(val), w(val)
		{
		}

		Vec4(T _x = 0, T _y = 0, T _z = 0, T _w = 0) : x(_x), y(_y), z(_z), w(_w)
		{
		}

		Vec4(const Vec3<T>& other, T value = 1.0f) : x(other.x), y(other.y), z(other.z), w(value)
		{
		}

		Vec4(const Vec4& other) : x(other.x), y(other.y), z(other.z), w(other.w)
		{
		}

		Vec4(Vec4&& other) : x(other.x), y(other.y), z(other.z), w(other.w)
		{
		}

		~Vec4() { }

		inline Vec4& operator=(const Vec4& other)
		{
			x = other.x;
			y = other.y;
			z = other.z;
			w = other.w;
			return *this;
		}

		inline Vec4& operator=(Vec4&& other)
		{
			x = other.x;
			y = other.y;
			z = other.z;
			w = other.w;
			return *this;
		}

		inline T Dot(const Vec4& other) const
		{
			return x * other.x + y * other.y + z * other.z + w * other.w;
		}

		inline Vec4 Cross(const Vec4& other) const
		{
			return {
				y * other.z - z * other.y,
				z * other.x - x * other.z,
				x * other.y - y * other.x,
				0
			};
		}

		inline T LengthSqr() const
		{
			return Dot(*this);
		}

		inline T Length() const
		{
			return sqrt(LengthSqr());
		}

		inline Vec4 GetNormalized() const
		{
			if (T l = Length(); l != 0)
			{
				T invlen = 1 / l;
				return { x * invlen, y * invlen, z * invlen, w * invlen };
			}
			return { 0, 0, 0, 0 };
		}

		inline void Normalize()
		{
			*this = GetNormalized();
		}

		inline bool operator==(const Vec4& other) const
		{
			return x == other.x && y == other.y && z == other.z && w == other.w;
		}

		inline bool operator!=(const Vec4& other) const
		{
			return x != other.x && y != other.y && z != other.z && w != other.w;
		}

		inline Vec4 operator+(const Vec4& other) const
		{
			return {
				x + other.x,
				y + other.y,
				z + other.z,
				w + other.w
			};
		}
		inline Vec4 operator-(const Vec4& other) const
		{
			return {
				x - other.x,
				y - other.y,
				z - other.z,
				w - other.w
			};
		}
		inline Vec4 operator*(const Vec4& other) const
		{
			return {
				x * other.x,
				y * other.y,
				z * other.z,
				w * other.w
			};
		}
		inline Vec4 operator/(const Vec4& other) const
		{
			return {
				x / other.x,
				y / other.y,
				z / other.z,
				w / other.w
			};
		}

		inline Vec4 operator*(const T& scalar) const
		{
			return {
				x * scalar,
				y * scalar,
				z * scalar,
				w * scalar
			};
		}
		inline Vec4 operator/(const T& scalar) const
		{
			return {
				x / scalar,
				y / scalar,
				z / scalar,
				w / scalar
			};
		}

		inline void operator+=(const Vec4& other)
		{
			*this = *this + other;
		}
		inline void operator-=(const Vec4& other)
		{
			*this = *this - other;
		}
		inline void operator*=(const Vec4& other)
		{
			*this = *this * other;
		}
		inline void operator/=(const Vec4& other)
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

		union
		{
			struct { T x, y, z, w; };
			struct { T r, g, b, a; };

			struct { Vec3<T> xyz; T w; };
			struct { T x; Vec3<T> yzw; };
		};
	};

	using Vec4f = Vec4<f32>;
	using Vec4i = Vec4<i32>;
	using Vec4u = Vec4<u32>;
}