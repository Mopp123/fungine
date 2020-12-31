#pragma once

#include "Vector2.h"

// "mml" stands for: "my math library"
namespace mml
{
	// *This can be used only as a container for integers..
	struct IVector3 : public IVector2
	{
		int z = 0;
		IVector3() {}
		IVector3(int X, int Y, int Z) : IVector2(X, Y), z(Z) {}
	};

	class Vector3
	{
	public:
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;

		Vector3() {}
		Vector3(float X, float Y, float Z);
		
		float magnitude() const;
		void normalize();

		float dot(const Vector3& other) const;
		Vector3 cross(const Vector3& other);


		friend Vector3 operator+(const Vector3& left, const Vector3& right);
		friend Vector3 operator-(const Vector3& left, const Vector3& right);
		friend Vector3 operator*(const Vector3& left, const Vector3& right);
		friend Vector3 operator*(const Vector3& left, float right);

	};
}