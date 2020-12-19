
#include "Vector3.h"
#include <cmath>

namespace mml
{
	Vector3::Vector3(float X, float Y, float Z) :
		x(X), y(Y), z(Z)
	{}

	float Vector3::magnitude() const
	{
		return std::sqrtf((x * x) + (y * y) + (z * z));
	}

	void Vector3::normalize()
	{
		float m = magnitude();
		if (m != 0.0f)
		{
			x /= m;
			y /= m;
			z /= m;
		}
	}

	float Vector3::dot(const Vector3& other) const
	{
		return (x * other.x + y * other.y + z * other.z);
	}

	Vector3 Vector3::cross(const Vector3& other)
	{
		return Vector3(
			y * other.z - z * other.y,
			-(x * other.z - z * other.x),
			x * other.y - y * other.x
		);
	}

	Vector3 operator+(const Vector3& left, const Vector3& right)
	{
		return Vector3(left.x + right.x, left.y + right.y, left.z + right.z);
	}

	Vector3 operator-(const Vector3& left, const Vector3& right)
	{
		return Vector3(left.x - right.x, left.y - right.y, left.z - right.z);
	}

	Vector3 operator*(const Vector3& left, const Vector3& right)
	{
		return Vector3(left.x * right.x, left.y * right.y, left.z * right.z);
	}

	Vector3 operator*(const Vector3& left, float right)
	{
		return Vector3(left.x * right, left.y * right, left.z * right);
	}
}