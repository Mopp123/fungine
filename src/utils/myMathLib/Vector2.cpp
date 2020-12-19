
#include "Vector2.h"
#include <cmath>

namespace mml
{
	Vector2::Vector2(float X, float Y) : 
		x(X), y(Y)
	{}

	float Vector2::magnitude() const
	{
		return std::sqrtf((x * x) + (y * y));
	}

	void Vector2::normalize()
	{
		float m = magnitude();
		if (m != 0.0f)
		{
			x /= m;
			y /= m;
		}
	}

	float Vector2::dot(const Vector2& other) const
	{
		return (x * other.x + y * other.y);
	}

	Vector2 operator+(const Vector2& left, const Vector2& right)
	{
		return Vector2(left.x + right.x, left.y + right.y);
	}

	Vector2 operator-(const Vector2& left, const Vector2& right)
	{
		return Vector2(left.x - right.x, left.y - right.y);
	}

	Vector2 operator*(const Vector2& left, const Vector2& right)
	{
		return Vector2(left.x * right.x, left.y * right.y);
	}

	Vector2 operator*(const Vector2& left, float right)
	{
		return Vector2(left.x * right, left.y * right);
	}
}