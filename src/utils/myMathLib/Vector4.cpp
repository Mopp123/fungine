
#include "Vector4.h"
#include <cmath>

namespace mml
{
	Vector4::Vector4(float X, float Y, float Z, float W) :
		x(X), y(Y), z(Z), w(W)
	{}

	float Vector4::magnitude() const
	{
            // Quick fix to get running on linux..
            #ifdef _WIN32
	    return std::sqrtf((x * x) + (y * y) + (z * z) + (w * w));
            #elif __linux__
	    return sqrtf((x * x) + (y * y) + (z * z) + (w * w));
            #else
            // error...
            #endif
	}

	void Vector4::normalize()
	{
		float m = magnitude();
		if (m != 0.0f)
		{
			x /= m;
			y /= m;
			z /= m;
			w /= m;
		}
	}

	float dot(const Vector4& left, const Vector4& right)
	{
		return (left.x * right.x + left.y * right.y + left.z * right.z * left.w * right.w);
	}

	Vector4 operator+(const Vector4& left, const Vector4& right)
	{
		return Vector4(left.x + right.x, left.y + right.y, left.z + right.z, left.w + right.w);
	}

	Vector4 operator-(const Vector4& left, const Vector4& right)
	{
		return Vector4(left.x - right.x, left.y - right.y, left.z - right.z, left.w - right.w);
	}

	Vector4 operator*(const Vector4& left, const Vector4& right)
	{
		return Vector4(left.x * right.x, left.y * right.y, left.z * right.z, left.w * right.w);
	}

	Vector4 operator*(const Vector4& left, float right)
	{
		return Vector4(left.x * right, left.y * right, left.z * right, left.w * right);
	}
}
