
#include "Quaternion.h"
#include "Vector3.h"
#include "Matrix4.h"
#include <cmath>

namespace mml
{

	Quaternion::Quaternion()
	{}

	Quaternion::Quaternion(float X, float Y, float Z, float W) : 
		x(X), y(Y), z(Z), w(W)
	{}

	// *Give angle in radians
	Quaternion::Quaternion(const mml::Vector3& axis, float angle)
	{
		float sinHalfAngle = std::sin(angle * 0.5f);
		x = axis.x * sinHalfAngle;
		y = axis.y * sinHalfAngle;
		z = axis.z * sinHalfAngle;
		w = 1.0f * std::cos(angle * 0.5f);
	}

	float Quaternion::magnitude() const
	{
		return std::sqrtf((x * x) + (y * y) + (z * z) + (w * w));
	}

	void Quaternion::normalize()
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

	mml::Quaternion Quaternion::conjugate() const
	{
		return mml::Quaternion(-x, -y, -z, w);
	}

	// Used wikipedia "Quat-derived rotation matrix"
	// This can only be used for "unit quaternion"
	Matrix4 Quaternion::toRotationMatrix() const
	{
		Quaternion q = *this;

		// We force the usage of unit quaternion here...
		q.normalize();

		float s = 1.0f;

		float sqx = q.x * q.x;
		float sqy = q.y * q.y;
		float sqz = q.z * q.z;

		Matrix4 rotationMatrix(1.0f);

		rotationMatrix[0] = 1.0f - 2 * s * (sqy + sqz);
		rotationMatrix[4] = 2 * s * (q.x * q.y - q.z * q.w);
		rotationMatrix[8] = 2 * s * (q.x * q.z + q.y * q.w);

		rotationMatrix[1] = 2 * s * (q.x * q.y + q.z * q.w);
		rotationMatrix[5] = 1.0f - 2 * s * (sqx + sqz);
		rotationMatrix[9] = 2 * s * (q.y * q.z - q.x * q.w);

		rotationMatrix[2] = 2 * s * (q.x * q.z - q.y * q.w);
		rotationMatrix[6] = 2 * s * (q.y * q.z + q.x * q.w);
		rotationMatrix[10] = 1.0f - 2 * s * (sqx + sqy);

		rotationMatrix[15] = 1.0f;

		return rotationMatrix;
	}


	Quaternion operator*(const Quaternion& left, const Quaternion& right)
	{
		return Quaternion(
			left.x * right.w + left.w * right.x + left.y * right.z - left.z * right.y,
			left.y * right.w + left.w * right.y + left.z * right.x - left.x * right.z,
			left.z * right.w + left.w * right.z + left.x * right.y - left.y * right.x,
			left.w * right.w - left.x * right.x - left.y * right.y - left.z * right.z
			);
	}

	Quaternion operator*(const Quaternion& left, const Vector3& right)
	{
		Quaternion q;
		q.w = -left.x * right.x - left.y * right.y - left.z * right.z;
		q.x = left.w * right.x + left.y * right.z - left.z * right.y;
		q.y = left.w * right.y + left.z * right.x - left.x * right.z;
		q.z = left.w * right.z + left.x * right.y - left.y * right.x;
		return q;
	}

	Quaternion operator*(const Quaternion& left, float right)
	{
		return Quaternion(left.x * right, left.y * right, left.z * right, left.w * right);
	}

	bool operator==(const Quaternion& left, const Quaternion& right)
	{
		return left.x == right.x && left.y == right.y && left.z == right.z && left.w == right.w;
	}
	
	bool operator!=(const Quaternion& left, const Quaternion& right)
	{
		return !(left == right);
	}

}