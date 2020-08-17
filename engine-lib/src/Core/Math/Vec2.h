#pragma once
#pragma warning(disable: 4201)

#include "Core/Types.h"

namespace fw
{
	template<typename T>
	class Vec2
	{
	public:
		Vec2(T val) : x(val), y(val)
		{
		}

		Vec2(T _x = 0, T _y = 0) : x(_x), y(_y)
		{
		}

		Vec2(const Vec2& other) : x(other.x), y(other.y)
		{
		}

		Vec2(Vec2&& other) : x(other.x), y(other.y)
		{
		}

		~Vec2() { }

		inline Vec2& operator=(const Vec2& other)
		{
			x = other.x;
			y = other.y;
			return *this;
		}

		inline Vec2& operator=(Vec2&& other)
		{
			x = other.x;
			y = other.y;
			return *this;
		}

		inline T Dot(const Vec2& other) const
		{
			return { x * other.x + y * other.y };
		}

		inline Vec2 Normal() const
		{
			return { -y, x };
		}

		inline T LengthSqr() const
		{
			return Dot(*this);
		}

		inline T Length() const
		{
			return sqrt(LengthSqr());
		}

		inline Vec2 GetNormalized() const
		{
			if (T l = Length(); l != 0)
			{
				T invlen = 1 / l;
				return { x * invlen, y * invlen };
			}
			return { 0,0 };
		}

		inline void Normalize()
		{
			*this = GetNormalized();
		}

		inline bool operator==(const Vec2& other) const
		{
			return x == other.x && y == other.y;
		}

		inline bool operator!=(const Vec2& other) const
		{
			return !((*this) == other);
		}

		inline Vec2 operator+(const Vec2& other) const
		{
			return {
				x + other.x,
				y + other.y
			};
		}
		inline Vec2 operator-(const Vec2& other) const
		{
			return {
				x - other.x,
				y - other.y
			};
		}
		inline Vec2 operator*(const Vec2& other) const
		{
			return {
				x * other.x,
				y * other.y
			};
		}
		inline Vec2 operator/(const Vec2& other) const
		{
			return {
				x / other.x,
				y / other.y
			};
		}
		inline Vec2 operator*(const T& scalar) const
		{
			return {
				x * scalar,
				y * scalar
			};
		}
		inline Vec2 operator/(const T& scalar) const
		{
			return {
				x / scalar,
				y / scalar
			};
		}

		inline void operator+=(const Vec2& other)
		{
			*this = *this + other;
		}
		inline void operator-=(const Vec2& other)
		{
			*this = *this - other;
		}
		inline void operator*=(const Vec2& other)
		{
			*this = *this * other;
		}
		inline void operator/=(const Vec2& other)
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

		T x, y;
	};

	using Vec2f = Vec2<f32>;
	using Vec2i = Vec2<i32>;
	using Vec2u = Vec2<u32>;
}