#pragma once

namespace mml
{
	class Vector3;
	class Matrix4;

	class Quaternion
	{
	public:
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
		float w = 0.0f;

		Quaternion();
		Quaternion(float X, float Y, float Z, float W);
		// *Give angle in radians
		Quaternion(const mml::Vector3& axis, float angle);

		float magnitude() const;
		void normalize();
		mml::Quaternion conjugate() const;

		Matrix4 toRotationMatrix() const;

		friend Quaternion operator*(const Quaternion& left, const Quaternion& right);
		friend Quaternion operator*(const Quaternion& left, const Vector3& right);
		friend Quaternion operator*(const Quaternion& left, float right);

		friend bool operator==(const Quaternion& left, const Quaternion& right);
		friend bool operator!=(const Quaternion& left, const Quaternion& right);
	};
}